#include "util.h"

exppS* create_opp_unaire(OPP o, exppS* e) {
    exppS* n = (exppS*) malloc (sizeof(exppS));
    n->type = opp_unaire;
    n->u.opp_u.opp = o;
    n->u.opp_u.op1 = e;
    return n;
}

exppS* create_opp_binaire(OPP o, exppS* e1, exppS* e2) {
    exppS* n = (exppS*) malloc (sizeof(exppS));
    n->type = opp_binaire;
    n->u.opp_b.opp = o;
    n->u.opp_b.op1 = e1;
    n->u.opp_b.op2 = e2;
    return n;
}

exppS* create_terme(char c) {
    exppS* n = (exppS*) malloc (sizeof(exppS));
    n->type = terme;
    char* ch = (char*) malloc (sizeof(char));
    (*ch) = c;
    n->u.t.c = ch;
    return n;
}

void free_exppS(exppS* e) {
    switch(e->type) {
        case opp_binaire:
            free_exppS(e->u.opp_b.op1);
            free_exppS(e->u.opp_b.op2);
            break;
        case opp_unaire:
            free_exppS(e->u.opp_u.op1);
            break;
        default:
            free(e->u.t.c);
    }
    free(e);
}

void display_opp(OPP o) {
    switch(o){
        case NON :
            printf("NON");
            break;
        case OU :
            printf("OU");
            break;
        case ET :
            printf("ET");
            break;
        case IMPLIQUE :
            printf("IMPLIQUE");
            break;
        case CARRE :
            printf("CARRE");
            break;
        case LOSANGE :
            printf("LOSANGE");
            break;
    }
}

void display_exppS(exppS* e) {
    switch(e->type) {
        case opp_unaire:
            display_opp(e->u.opp_u.opp);
            printf("(");
            display_exppS(e->u.opp_u.op1);
            printf(")");
            break;
        case opp_binaire:
            display_opp(e->u.opp_b.opp);
            printf("(");
            display_exppS(e->u.opp_b.op1);
            printf(",");
            display_exppS(e->u.opp_b.op2);
            printf(")");
            break;
        case terme:
            printf("%c", (*e->u.t.c));
            break;
    }
}
