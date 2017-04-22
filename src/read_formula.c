#include "read_formula.h"

void conso(char* formula, int* p);

exppS* opp_impl(char* formula, int* p, exppS* h);
exppS* opp_ou(char* formula, int* p, exppS* h);
exppS* opp_et(char* formula, int* p, exppS* h);
exppS* opp_b(char* formula, int *p,exppS * herite);
exppS* expp_Par(char *formula, int *p);
exppS* unaire(char* formula, int* p, OPP o);
int isOpp_b(char* formula, int*p);
int isOpp_u(char* formula, int *p);
exppS* opp_Car(char *formula, int *p);
exppS* opp_los(char *formula, int* p);
exppS* opp_u(char* formula, int* p);
exppS* expp_n(char* formula, int* p);
exppS* expp(char* formula, int* p);

void eat_char(char* formula, int* p) {
    while(formula[*p] == ' ' || formula[*p] == '\n' || formula[*p] == '\t') (*p)++;
}

void conso(char* formula, int* p) {
    eat_char(formula, p);
    (*p)++;
    eat_char(formula, p);
}

exppS* opp_impl(char* formula, int* p, exppS* h) {
    if (formula[*p] == '-') {
        conso(formula, p);
        if (formula[*p] == '>') {
            conso(formula, p);
            exppS* r = expp_n(formula, p);
            exppS* impl = create_opp_binaire(IMPLIQUE, h, r);
            return opp_b(formula, p, impl);
        } else {
            printf("ERRR: attendu < \'>\' >\n");
            exit(1);
        }
    }
    return h;
}

exppS* opp_ou(char* formula, int* p, exppS* h) {
    if (formula[*p] == '+') {
        conso(formula, p);
        exppS* r = expp_n(formula, p);
        exppS* ou = create_opp_binaire(OU, h, r);
        return opp_b(formula, p,ou);
    }
    return opp_impl(formula, p, h);
}

exppS* opp_et(char* formula, int* p, exppS* h) {
    if (formula[*p] == '*') {
        conso(formula, p);
        exppS* r = expp_n(formula, p);
        exppS* et = create_opp_binaire(ET, h, r);
        return opp_b(formula, p,et);
    }
    return opp_ou(formula, p, h);
}

exppS* opp_b(char* formula, int *p,exppS * herite) {
    return opp_et(formula, p, herite);
}

exppS* termeInf(char* formula, int* p) {
    exppS* t = create_terme(formula[*p]);
    conso(formula, p);
    return t;
}

exppS* expp_Par(char *formula, int *p) {
    conso(formula, p);
    exppS *r = expp_n(formula, p);
    if(formula[*p] == ')') {
        conso(formula, p);
    } else {
        printf("ERRR: attendu ) \n");
        exit(1);
    }
    return r;
}

exppS* unaire(char* formula, int* p, OPP o) {
    exppS* t = NULL;
    if (is_alpha(formula[*p])) {
        t = termeInf(formula, p);
    } else if(formula[*p] == '(') {
        t = expp_Par(formula, p);
    } else if(isOpp_u(formula, p)){
        t = opp_u(formula,p);
    } 
    else {
        printf("ERRR: attendu ( ou \'VAR\' \n");
        exit(1);
    }
    exppS* r = create_opp_unaire(o, t);
    return r;
}

int isOpp_b(char* formula, int*p) {
    return (formula[*p] == '-' || formula[*p] == '+' || formula[*p] == '*' );
}

int isOpp_u(char* formula, int *p) {
    return (formula[*p] == '/' || formula[*p] == '[' || formula[*p] == '<' );
}

exppS* opp_Non(char *formula, int *p) {
    conso(formula, p);
    return unaire(formula, p, NON);
}

exppS* opp_Car(char *formula, int *p) {
    conso(formula, p);
    if (formula[*p] == ']') {
        conso(formula, p);
        return unaire(formula, p, CARRE);
    }
    printf("ERRR: attendu < ] >\n");
    exit(1);
}

exppS* opp_los(char *formula, int* p) {
    conso(formula, p);
    if (formula[*p] == '>') {
        conso(formula, p);
        return unaire(formula, p, LOSANGE);
    }
    printf("ERRR: attendu < \'<\' >\n");
    exit(1);
}

exppS* opp_u(char* formula, int* p) {
    if (formula[*p] == '/') {
        return opp_Non(formula, p);
    } else if (formula[*p] == '[') {
        return opp_Car(formula, p);
    }
    return opp_los(formula, p);
}

exppS* expp_n(char* formula, int* p) {
    if (formula[*p] == '(') {
        exppS* h = expp_Par(formula, p);
        return opp_b(formula, p, h);
    } else if(isOpp_u(formula, p)) {
        exppS* h = opp_u(formula, p);
        return opp_b(formula, p, h);
    } else if(is_alpha(formula[*p])) {
        exppS* h  = termeInf(formula, p);
        return opp_b(formula, p, h);
    }
    printf("ERREUR attendu '(' VAR \n");;
    exit(1);
}

exppS* expp(char* formula, int* p) {
    if (formula[*p] == '(') {
        exppS* h = expp_Par(formula, p);
        return opp_b(formula, p, h);
    }
    else if(isOpp_u(formula, p)) {
        exppS* h = opp_u(formula, p);
        return opp_b(formula, p, h);

    } else if(is_alpha(formula[*p])) {
        exppS* h  = termeInf(formula, p);
        return opp_b(formula, p, h);
    }
    printf("ERREUR attendu '(' VAR \n");;
    exit(1);
}

exppS* read_formula(char* formula) {
    int p = 0;
    return expp(formula, &p);
}
