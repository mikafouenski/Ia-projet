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


///////// Suite


// int addLiteral(litteraux *litterauxFind,exppS* e,int monde, int size, int sizeTab){
//     if(size > sizeTab){
//         //realoc si nécéssaire
//     }
//     litterauxFind[size].e = e;
//     litterauxFind[size].monde = monde;
//     ++size;
//     return size;
// }

// int check_validiti(exppS* e, litteraux *litterauxFind,int size, int sizeTab,int monde){
//     //On ajoute un terme positif on regarde si un négatif contredi

//     if(e->type == terme){

//         for (int i = 0; i < size; ++i)
//         {
//             if(litterauxFind[i].monde == monde && litterauxFind[i].e->type == opp_unaire){
//                 if(strcmp(litterauxFind[i].e->u.opp_u.op1->u.t.c,e->u.t.c) == 0){
//                     return -1;
//                 }
//             }
//         }
//     }
//     //On ajoute un NON
//     else {
//         char* c = e->u.opp_u.op1->u.t.c;
//         for (int i = 0; i < size; ++i)
//         {
//             if(litterauxFind[i].monde == monde && litterauxFind[i].e->type == terme){
//                 if(strcmp(c,litterauxFind[i].e->u.t.c) == 0){
//                     return -1;
//                 }
//             }
//         }
//     }

//     return addLiteral(litterauxFind,e,monde,size,sizeTab);;
// }

// int testUnaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);
// int testBinaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);
// int rulesNON(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);

// int test(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
//     if(e->type == opp_unaire)
//         return testUnaire(e,litterauxFind,size,sizeTab,monde);
//     else if(e->type == opp_binaire)
//         return testBinaire(e,litterauxFind,size,sizeTab,monde);
//     else{
//         // for (int i = 0; i < size; ++i)
//         // {
//         //      display_exppS(litterauxFind[i].e);
//         // }
//         // printf("\n");
//         // display_exppS(e);
//         // printf("\n");
//         int test = check_validiti(e,litterauxFind,size,sizeTab,monde);
//         // printf("test %d\n",test);
//         return test;
//     }
// }

// int rulesNON(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
//     // if(size < 0)
//     //     return size;
//     if(e->type == opp_unaire){
//         // //a -> a
//         if(e->u.opp_u.opp == NON){
//             return test(e->u.opp_u.op1,litterauxFind,size,sizeTab,monde);
//         }
//         return -1;
//     }
//     else if(e->type == opp_binaire){
//         ///(A V B) -> X: /a /b
//         if(e->u.opp_b.opp == OU){
//             exppS * t = create_opp_unaire(NON, e->u.opp_b.op1);
//             size = test(t,litterauxFind,size,sizeTab,monde);
//             free(t);
//             if(size < 0)
//                 return size;
//             t = create_opp_unaire(NON, e->u.opp_b.op2);
//             return test(t,litterauxFind,size,sizeTab,monde);
//         }

//         else if(e->u.opp_b.opp == ET){
//             int temp;
//             exppS * t = create_opp_unaire(NON, e->u.opp_b.op1);
//             temp = test(t,litterauxFind,size,sizeTab,monde);

//             if(temp < 0){
//                 free(t);
//                 t = create_opp_unaire(NON, e->u.opp_b.op2);
//                 return test(t,litterauxFind,size,sizeTab,monde);
//             }
//             return temp;
//         }
//         return -1;
//     }
//     else{
//         exppS * t = create_opp_unaire(NON, e);
//         int test = check_validiti(t,litterauxFind,size,sizeTab,monde);
//         free(t);
//         return test;
//     }
// }


// int testUnaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
//     if(e->u.opp_u.opp == NON){
//         return rulesNON(e->u.opp_u.op1, litterauxFind,size,sizeTab,monde);
//     }

//     printf("Pas encore fait\n");
//     return 0;
// }

// int testBinaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){

//     if(e->u.opp_b.opp == OU){
//         int temp;

//         temp = test(e->u.opp_b.op1,litterauxFind,size,sizeTab,monde);
//         if(temp < 0){
//             return test(e->u.opp_b.op2,litterauxFind,size,sizeTab,monde);
//         }
//         return temp;
//     }

//     if(e->u.opp_b.opp == ET){
//         int temp;

//         temp = test(e->u.opp_b.op1,litterauxFind,size,sizeTab,monde);
//         if(temp < 0){
//             return temp;
//         }
//         return test(e->u.opp_b.op2,litterauxFind,temp,sizeTab,monde);
//     }


//     printf("Pas encore fait\n");
//     return 0;
// }


// ///////////////// SUITE

branch* create_branch(exppS* e, int monde, int end_deriv) {
    branch* b = (branch*) malloc(sizeof(branch));
    b->nexts[0] = NULL;
    b->nexts[1] = NULL;
    b->e = e;
    b->end_deriv = end_deriv;
    b->monde = monde;
    return b;
}

void display_branch(branch* b, int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
    if (b->e != NULL) {
        display_exppS(b->e);
        printf("  M:%d  END:%d\n", b->monde, b->end_deriv);
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

int is_end_deriv(exppS* e) {
    return is_terme(e) || is_non_terme(e);
}

char* get_end_terme(exppS* e) {
    if (is_terme(e))
        return e->u.t.c;
    else if (is_non_terme(e))
        return e->u.opp_u.op1->u.t.c;
    printf("ERREUR get_end_terme\n");
    exit(1);
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
        for (int i = 0; i < size; ++i)
        {
            if(litterauxFind[i].monde == monde && litterauxFind[i].e->type == opp_unaire){
                if(strcmp(litterauxFind[i].e->u.opp_u.op1->u.t.c,e->u.t.c) == 0){
                    return -1;
                }
            }
        }
    }
    //On ajoute un NON
    else {
        char* c = e->u.opp_u.op1->u.t.c;
        for (int i = 0; i < size; ++i)
        {
            if(litterauxFind[i].monde == monde && litterauxFind[i].e->type == terme){
                if(strcmp(c,litterauxFind[i].e->u.t.c) == 0){
                    return -1;
                }
            }
        }
    }
    return addLiteral(litterauxFind,e,monde,size);;
}

// //a = a
int rule1(branch* b) {
    if (test_opp(b->e, NON) && is_non_terme(b->e->u.opp_u.op1)) { // j'ai trouver //a
        printf("rule1\n");
        exppS* t = (exppS*) malloc(sizeof(exppS));
        memcpy(t, b->e->u.opp_u.op1->u.opp_u.op1, sizeof(exppS));
        b->nexts[0] = create_branch(t, b->monde, 1);
    }
    return 0;
}

// a^b = a  b
int rule2(branch* b) {
    if (test_opp(b->e, ET)) { // j'ai trouver a^b
        printf("rule2\n");
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        b->nexts[0] = create_branch(op1, b->monde, is_end_deriv(op1));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        b->nexts[1] = create_branch(op2, b->monde, is_end_deriv(op2));
    }
    return 0;
}

// /(A^B) = /a | /b     -> on branche
int rule6(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, ET)) { // j'ai trouver /(a^b)
        printf("rule6\n");
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        b->nexts[0] = create_branch(non_op1, b->monde, is_end_deriv(non_op1));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        b->nexts[1] = create_branch(non_op2, b->monde, is_end_deriv(non_op2));
    }
    return 0;
}

int ruleConflit(branch* b,litteraux* litterauxFind,int size){
        if(is_terme(b->e) || is_non_terme(b->e))
            return check_validity(b->e,litterauxFind,size,b->monde);
        return 0;
}




// retourne 1 si tautologie
int deriv_back(branch* b, rule* sys, int sys_size, litteraux* litterauxFind, int size) {
    for (int i = 0; i < sys_size; ++i) {
        (*sys[i])(b);
    }
    size = ruleConflit(b,litterauxFind,size);
    if(size < 0){
        return 0;
    }
    int r;
    if (b->nexts[0] != NULL) {
        r = deriv_back(b->nexts[0], sys, sys_size,litterauxFind,size);
    }
    //Branche valide
    if(r > 0)
        return 1;
    if (b->nexts[1] != NULL) {
        r = deriv_back(b->nexts[1], sys, sys_size, litterauxFind,size);
    }
    return r;
}

// retourne 1 si tautologie
int deriv(exppS* e) {
    branch* head = create_branch(e, 0, 0);
    litteraux* litterauxFind = malloc(sizeof(litteraux) * 100);
    rule system_K[3] = {rule1, rule2, rule6};
    int system_K_size = 3;
    int r = deriv_back(head, system_K, system_K_size,litterauxFind, 0);
    display_branch(head, 0);
    return r;
}