#include "modal.h"

void eat_char(char* buf, int* p) {
    while(buf[*p] == ' ' || buf[*p] == '\n' || buf[*p] == '\t') (*p)++;
}

void conso(char* buf, int* p) {
    eat_char(buf, p);
    (*p)++;
    eat_char(buf, p);
}

exppS* opp_impl(char* buf, int* p, exppS* h) {
     if (buf[*p] == '-') {
        conso(buf, p);
        if (buf[*p] == '>') {
            conso(buf, p);
            exppS* r = expp(buf, p, NULL);
            exppS* impl = create_opp_binaire(IMPLIQUE, h, r);
            return impl;
        } else {
            printf("ERRR: attendu < \'>\' >\n");
            exit(1);
        }
    }
    return opp_ou(buf, p, h);
}

exppS* opp_ou(char* buf, int* p, exppS* h) {
    if (buf[*p] == '+') {
        conso(buf, p);
        exppS* r = expp(buf, p, NULL);
        exppS* ou = create_opp_binaire(OU, h, r);
        return ou;
    }
    return opp_et(buf, p, h);
}

exppS* opp_et(char* buf, int* p, exppS* h) {
    if (buf[*p] == '*') {
        conso(buf, p);
        exppS* r = expp(buf, p, NULL);
        exppS* et = create_opp_binaire(ET, h, r);
        return et;
    }
    return h;
}

exppS* termeInf(char* buf, int* p) {
    exppS* t = create_terme(buf[*p]);
    conso(buf, p);
    return t;
}

exppS* unaire(char* buf, int* p, OPP o) {
    exppS* t = NULL;
    if (is_alpha(buf[*p])) t = termeInf(buf, p);
    else t = expp(buf, p, NULL);
    exppS* non = create_opp_unaire(o, t);
    return expp(buf, p, non);
}

exppS* expp(char* buf, int* p, exppS *herite) {
    if (buf[*p] == '/') {
        conso(buf, p);
        return unaire(buf, p, NON);
    } else if (buf[*p] == '[') {
        conso(buf, p);
        if (buf[*p] == ']') {
            conso(buf, p);
            return unaire(buf, p, CARRE);
        } else {
            printf("ERRR: attendu < ] >\n");
            exit(1);
        }
    } else if (buf[*p] == '<') {
        conso(buf, p);
        if (buf[*p] == '>') {
            conso(buf, p);
            return unaire(buf, p, LOSANGE);
        } else {
            printf("ERRR: attendu < \'<\' >\n");
            exit(1);
        }
    } else if (buf[*p] == '(') {
        conso(buf, p);
        exppS *r = expp(buf, p,herite);
        conso(buf, p);
        return r;
    } else if (is_alpha(buf[*p])) {
        return expp(buf, p, termeInf(buf,p));
    } else {
        exppS* r = opp_impl(buf, p, herite);
        return r;
    }
    return herite;
}

exppS* negate_exppS(exppS* e) {
    exppS* n = create_opp_unaire(NON, e);
    return n;
}

// exppS* propagation_not(exppS* e, int invert) { // Bool pour l'inversion des signes
//     switch(e->type) {
//         case opp_unaire:
//             if (e->u.opp_u.opp == NON) {
//                 exppS* n = e->u.opp_u.op1;
//                 free(e);
//                 return propagation_not(n, 1);
//             }
//             break;
//         case opp_binaire:
//             if (invert) {
//                 switch(e->u.opp_u.opp) {
//                     case OU:
//                         e->u.opp_u.opp = ET;
//                         break;
//                     case ET:
//                         e->u.opp_u.opp = OU;
//                         break;
//                     case IMPLIQUE:
//                         break;
//                 }
//             }
//             e->u.opp_u.op1 = propagation_not(e->u.opp_u.op1, 0);
//             e->u.opp_u.op2 = propagation_not(e->u.opp_u.op2, 0);
//             return e;
//         case terme:

//             break;
//     }
// }

exppS* not_rule(exppS* e) {
    switch(e->type) {
        case opp_unaire:
            switch(e->u.opp_u.opp) {
                case NON:
                    return e->u.opp_u.op1;
                case CARRE:
                    printf("LE NOUVEAU MONDE !\n");
                case LOSANGE:
                    printf("UN LOSANGE ?\n");
            }
            break;
        case opp_binaire:
            switch(e->u.opp_u.opp) {
                case OU:
                    int appel_recusif = fonctionInconnue()
                case ET:
                case IMPLIQUE:
            }
        case terme:
    }
}
