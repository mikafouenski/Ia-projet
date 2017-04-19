#include "read_formula.h"

#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c))

void consomate(char* formula, int* p) {
    (*p)++;
}

void display_error(char* s) {
    fprintf(stderr, "ERREUR %s : cas non prévus", s);
    exit(1);
}

exppS* expression(char* formula, int* p);

exppS* var(char* formula, int* p) {
    if (is_alpha(formula[*p])) {
        exppS* result = create_terme(formula[*p]);
        consomate(formula, p);
        return result;
    } else if (formula[*p] == '(') {
        exppS* result = expression(formula, p);
        if (formula[*p] != ')') {
            free_exppS(result);
            printf("ERREUR var\n");
            exit(1);
        }
        consomate(formula, p);
        return result;
    }
    display_error("VAR");
    return NULL;
}

exppS* impl_b(char* formula, int* p, exppS* herite) {
    if (formula[*p] == '-' && formula[(*p) + 1] == '>') {
        consomate(formula, p);
        consomate(formula, p);
        exppS* _var = var(formula, p);
        exppS* h = create_opp_binaire(IMPLIQUE, herite, _var);
        return impl_b(formula, p, h);
    }
    return herite;
}

exppS* impl(char* formula, int* p) {
    if (is_alpha(formula[*p])) {
        exppS* _var = var(formula, p);
        if (formula[*p] != '-' || formula[(*p) + 1] != '>') {
            free_exppS(_var);
            printf("ERREUR impl\n");
            exit(1);
        }
        return impl_b(formula, p, _var);
    }
    display_error("IMPL");
    return NULL;
}

exppS* ou_b(char* formula, int* p, exppS* herite) {
    if (formula[*p] == '+') {
        consomate(formula, p);
        exppS* _impl = impl(formula, p);
        exppS* h = create_opp_binaire(OU, herite, _impl);
        return ou_b(formula, p, h);
    }
    return herite;
}

exppS* ou(char* formula, int* p) {
    if (formula[*p] == '-' || formula[(*p) + 1] == '>') {
        exppS* _impl = impl(formula, p);
        if (formula[*p] != '+') {
            free_exppS(_impl);
            printf("ERREUR ou\n");
            exit(1);
        }
        return ou_b(formula, p, _impl);
    }
    display_error("OU");
    return NULL;
}

exppS* et_b(char* formula, int* p, exppS* herite) {
    if (formula[*p] == '*') {
        consomate(formula, p);
        exppS* _ou = ou(formula, p);
        exppS* h = create_opp_binaire(ET, herite, _ou);
        return et_b(formula, p, h);
    }
    return herite;
}

exppS* et(char* formula, int* p) {
    if (formula[*p] == '+') {
        exppS* _ou = ou(formula, p);
        if (formula[*p] != '*') {
            free_exppS(_ou);
            printf("ERREUR et\n");
            exit(1);
        }
        return et_b(formula, p, _ou);
    }
    display_error("ET");
    return NULL;
}

exppS* losange(char* formula, int* p) {
    if (formula[*p] == '<' && formula[(*p) + 1] == '>') {
        consomate(formula, p);
        consomate(formula, p);
        exppS* _et = et(formula, p);
        return create_opp_unaire(LOSANGE, _et);
    }
    return et(formula, p);
}

exppS* carre(char* formula, int* p) {
    if (formula[*p] == '[' && formula[(*p) + 1] == ']') {
        consomate(formula, p);
        consomate(formula, p);
        exppS* _losange = losange(formula, p);
        return create_opp_unaire(CARRE, _losange);
    }
    return losange(formula, p);
}

exppS* non(char* formula, int* p) {
    if (formula[*p] == '/') {
        consomate(formula, p);
        exppS* _carre = carre(formula, p);
        return create_opp_unaire(NON, _carre);
    }
    return carre(formula, p);
}

exppS* expression(char* formula, int* p) {  // juste un wrapper
    return non(formula, p);
}


exppS* read_formula(char* formula) {
    int p = 0;
    return expression(formula, &p);
}
