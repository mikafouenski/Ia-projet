#include "modal.h"

exppS* opp_impl(char* buf, int* p, exppS* h);
exppS* opp_ou(char* buf, int* p, exppS* h);
exppS* opp_et(char* buf, int* p, exppS* h);
exppS* expp(char* buf, int* p, exppS * a);
exppS* create_opp_unaire(OPP o, exppS* e);
exppS* create_opp_binaire(OPP o, exppS* e1, exppS* e2);
exppS* create_terme(char c);


void read_file(const char* name, char* buf) {
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }
    if (! fscanf(f, "%s", buf)) {
        perror("fscanf");
        exit(1);
    }
    fclose(f);
}

void eat_char(char* buf, int* p) {
    while(buf[*p] == ' ' || buf[*p] == '\n' || buf[*p] == '\t') (*p)++;
}

void conso(int* p) {
    (*p)++;
}


exppS* opp_impl(char* buf, int* p, exppS* h) {
     if (buf[*p] == '-') {
        conso(p);
        if (buf[*p] == '>') {
            conso(p);
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
        conso(p);
        exppS* r = expp(buf, p, NULL);
        exppS* ou = create_opp_binaire(OU, h, r);
        return ou;
    }
    return opp_et(buf, p, h);
}

exppS* opp_et(char* buf, int* p, exppS* h) {
    if (buf[*p] == '*') {
        conso(p);
        exppS* r = expp(buf, p, NULL);
        exppS* et = create_opp_binaire(ET, h, r);
        return et;
    }
    return h;
}

exppS * termeInf(char* buf, int* p){
        exppS* t = create_terme(buf[*p]);
        conso(p);
        return t;
}

exppS* expp(char* buf, int* p, exppS *herite) {
    if (buf[*p] == '/') {
        conso(p);
        exppS *t = NULL;
        if (is_alpha(buf[*p])){
            t = termeInf(buf,p);
        }
        else 
            t = expp(buf, p,NULL);
        exppS *non = create_opp_unaire(NON,t);

        return expp(buf,p,non);
    
    } else if (buf[*p] == '[') {
        conso(p);
        if (buf[*p] == ']') {
            conso(p);
            expp(buf, p,NULL);
        } else {
            printf("ERRR: attendu < ] >\n");
            exit(1);
        }
    } else if (buf[*p] == '<') {
        conso(p);
        if (buf[*p] == '>') {
            conso(p);
            expp(buf, p,NULL);
        } else {
            printf("ERRR: attendu < \'<\' >\n");
            exit(1);
        }
    } else if (buf[*p] == '(') {
        conso(p);
        exppS *r = expp(buf, p,herite);
        conso(p);
        return r;
    } else if (is_alpha(buf[*p])) {

        return expp(buf,p,termeInf(buf,p));
        // exppS* t = create_terme(buf[*p]);
        // conso(p);
        // 
    } else {
        exppS* r = opp_impl(buf, p, herite);
        return r;
        // printf("ERRR: attendu < / | ( | char > \n");
        // exit(1);
    }
    return herite;
    }

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


void display_opp(OPP o){
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
            printf("%s", e->u.t.c);
            break;
    }
}

int main(int argc, char const *argv[]) {
    if (argc - 1 != 1) {
        printf("usage: %s FileName\n", argv[0]);
        exit(1);
    }

    char *buf = (char*) malloc (BUFFSIZE * sizeof(char));

    read_file(argv[1], buf);

    int p = 0;
    exppS* r = expp(buf, &p, NULL);

    display_exppS(r);
    printf("\n");

    printf("%s\n", buf);

    free(buf);
    free_exppS(r);
    return 0;
}
