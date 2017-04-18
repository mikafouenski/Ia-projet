#include "modal.h"
#include <string.h>

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
        return expp(buf, p,r);
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

// ///////////////// SUITE


void add_in_branch(branch* b, int indi, branch* add){
    branch * elm = b;
    while(elm->nexts[indi]){
        elm = elm->nexts[indi];
    }
    elm->nexts[indi]= add;

}

branch* create_branch(exppS* e, int monde) {
    branch* b = (branch*) malloc(sizeof(branch));
    b->nexts[0] = NULL;
    b->nexts[1] = NULL;
    b->e = e;
    b->monde = monde;
    return b;
}

void display_branch(branch* b, int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
    if (b->e != NULL) {
        display_exppS(b->e);
        printf("  M:%d\n", b->monde);
    }
    if (b->nexts[0]) display_branch(b->nexts[0], indent + 1);
    if (b->nexts[1]) display_branch(b->nexts[1], indent + 1);
}

int test_opp(exppS* e, OPP o) {  // renvoie 1 si opp = o
    if (e->type != terme) {
        switch(o) {
            case CARRE:
            case LOSANGE:
            case NON:
                return e->u.opp_u.opp == o;
            case OU:
            case ET:
            case IMPLIQUE:
                return e->u.opp_b.opp == o;
        }
    }
    return 0;
}

int is_terme(exppS* e) {
    return e->type == terme;
}

int is_non_terme(exppS* e) {
    return test_opp(e, NON) && is_terme(e->u.opp_u.op1);
}

int addLiteral(litteraux *litterauxFind,exppS* e,int monde, int size){
    litterauxFind[size].e = e;
    litterauxFind[size].monde = monde;
    ++size;
    return size;
}

int check_validity(exppS* e, litteraux *litterauxFind,int size,int monde){
    //On ajoute un terme positif on regarde si un négatif contredi
    if(e->type == terme){
        printf("%s\n",e->u.t.c);
        for (int i = 0; i < size; ++i)
        {
            printf("non %s",litterauxFind[i].e->u.opp_u.op1->u.t.c);
            if(litterauxFind[i].monde == monde && litterauxFind[i].e->type == opp_unaire){
                if(strcmp(litterauxFind[i].e->u.opp_u.op1->u.t.c,e->u.t.c) == 0){
                    printf("....\n");
                    return -1;
                }
            }
        }
    }
    //On ajoute un NON
    else {
        char* c = e->u.opp_u.op1->u.t.c;
        printf("non %s\n",c);
        for (int i = 0; i < size; ++i)
        {
            if(litterauxFind[i].monde == monde && litterauxFind[i].e->type == terme){
                printf("%s",litterauxFind[i].e->u.t.c);
                if(strcmp(c,litterauxFind[i].e->u.t.c) == 0){
                    printf("....\n");
                    return -1;
                }
            }
            printf("\n");
        }
    }
    return addLiteral(litterauxFind,e,monde,size);
}

// //a = a
void rule1(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, NON)) { // j'ai trouver //a
        exppS* t = (exppS*) malloc(sizeof(exppS));
        memcpy(t, b->e->u.opp_u.op1->u.opp_u.op1, sizeof(exppS));
        add_in_branch(b,0,create_branch(t, b->monde));
    }
}

// a^b = a  b
void rule2(branch* b) {
    if (test_opp(b->e, ET)) { // j'ai trouver a^b
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        add_in_branch(b,0,create_branch(op1, b->monde));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        add_in_branch(b,1,create_branch(op2, b->monde));
    }
}

// /(AvB) = /a  /b
void rule3(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, OU)) { // /(AvB)
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        add_in_branch(b,0,create_branch(non_op1, b->monde));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        add_in_branch(b,0,create_branch(non_op2, b->monde));
    }
}

// /(A->B) = a  /b
void rule4(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, IMPLIQUE)) { // /(A->B)
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        add_in_branch(b,0,create_branch(op1, b->monde));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        add_in_branch(b,0,create_branch(non_op2, b->monde));
    }
}

// AvB = a | b     -> on branche
void rule5(branch* b) {
    if (test_opp(b->e, OU)) { // j'ai trouver AvB
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        add_in_branch(b,0,create_branch(op1, b->monde));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        add_in_branch(b,1,create_branch(op2, b->monde));
    }
}

// /(A^B) = /a | /b     -> on branche
void rule6(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, ET)) { // j'ai trouver /(a^b)
        printf("rule6\n");
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        add_in_branch(b,0,create_branch(non_op1, b->monde));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        add_in_branch(b,1,create_branch(non_op2, b->monde));
    }
}

int ruleConflit(branch* b,litteraux* litterauxFind,int size){
    if(is_terme(b->e) || is_non_terme(b->e)){
        return check_validity(b->e,litterauxFind,size,b->monde);
    }
    return 0;
}

// A->B = /a | b     -> on branche
void rule7(branch* b) {
    if (test_opp(b->e, IMPLIQUE)) { // j'ai trouver A->B
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        add_in_branch(b,0,create_branch(non_op1, b->monde));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        add_in_branch(b,1,create_branch(op2, b->monde));
    }
}


int nbWorld = 0;

void ruleWorld(branch* b,int** worldFind) {
    // printf("nbWorld: %d\n",nbWorld);
    int bool = 0; 
    if(test_opp(b->e,CARRE)) {
        branch *c = b;
        while(c){
            if(!test_opp(c->e,CARRE)){
                add_in_branch(b,0,create_branch(b->e,b->monde));
                return;
            }
            c = c->nexts[0];
        }
        for (int i = 0; i < nbWorld+1; ++i){
            if(worldFind[b->monde][i] == 1){
                add_in_branch(b,0,create_branch(b->e->u.opp_u.op1,worldFind[b->monde][i]));
                bool = 1;
            }
        }

        
        
    }
}



int rulesCreateWorld(branch *b,int** worldFind){
    if (test_opp(b->e,NON) && test_opp(b->e->u.opp_u.op1, CARRE)) {
        printf("pk\n");
        ++nbWorld;
        worldFind[b->monde][nbWorld] = 1;
        exppS* non_op1 = negate_exppS((b->e->u.opp_u.op1->u.opp_u.op1));
        add_in_branch(b,0,create_branch(non_op1,nbWorld));
    }
    return nbWorld;   
}


// retourne 1 si tautologie
int deriv_back(branch* b, rule* sys, int sys_size, litteraux* litterauxFind, int size, int** worldFind, int nbWorld) {
    for (int i = 0; i < sys_size; ++i) (*sys[i])(b); // appel de toutes les règles du système sys
    int temp = ruleConflit(b,litterauxFind,size);
    if (temp < 0){
        return -1;
    }
    if(temp > 0)
        size = temp;

    ruleWorld(b,worldFind);
    nbWorld = rulesCreateWorld(b,worldFind);
    
    int r = 0;
    if (b->nexts[0] != NULL) {
        r = deriv_back(b->nexts[0], sys, sys_size,litterauxFind,size,worldFind,nbWorld);
    }
    //Branche valide
    if(r > 0){
        printf("Branche juste trouver\n");
        return 1;
    }
    if (b->nexts[1] != NULL) {
        r = deriv_back(b->nexts[1], sys, sys_size, litterauxFind,size,worldFind,nbWorld);
    }
    return r;
}

void free_branch(branch* b) {
    if (b->nexts[0])
        free_branch(b->nexts[0]);
    if (b->nexts[1])
        free_branch(b->nexts[1]);
    free(b);
}

// retourne 1 si tautologie
int deriv(exppS* e) {
    branch* head = create_branch(e, 0);
    litteraux* litterauxFind = (litteraux *)malloc(sizeof(litteraux) * 100);
    int system_K_size = 7;
    rule system_K[7] = {rule1, rule2, rule3, rule4, rule5, rule6, rule7}; // voila c'est mieux
    int** worldFind = (int**)malloc(sizeof(int*) * 100);
    for (int i = 0; i < 100; ++i)
    {
        worldFind[i] = (int*)malloc(sizeof(int)*100);
    } 
    int r = deriv_back(head, system_K, system_K_size,litterauxFind, 0,worldFind,0);
    display_branch(head, 0);
    free_branch(head);
    free(litterauxFind);
    return r;
}