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


int addLiteral(litteraux *litterauxFind,exppS* e,int monde, int size, int sizeTab){
    if(size > sizeTab){
        //realoc si nécéssaire 
    }
    litterauxFind[size].e = e;
    litterauxFind[size].monde = monde;
    ++size;
    return size;
}

int check_validiti(exppS* e, litteraux *litterauxFind,int size, int sizeTab,int monde){
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

    return addLiteral(litterauxFind,e,monde,size,sizeTab);; 
}

int testUnaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);
int testBinaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);
int rulesNON(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);

int test(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
    if(e->type == opp_unaire)
        return testUnaire(e,litterauxFind,size,sizeTab,monde);
    else if(e->type == opp_binaire)
        return testBinaire(e,litterauxFind,size,sizeTab,monde);
    else{
        // for (int i = 0; i < size; ++i)
        // {
        //      display_exppS(litterauxFind[i].e);
        // }
        // printf("\n");
        // display_exppS(e);
        // printf("\n");
        int test = check_validiti(e,litterauxFind,size,sizeTab,monde);
        // printf("test %d\n",test);
        return test;
    }
}

int rulesNON(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
    // if(size < 0)
    //     return size;
    if(e->type == opp_unaire){
        // //a -> a 
        if(e->u.opp_u.opp == NON){
            return test(e->u.opp_u.op1,litterauxFind,size,sizeTab,monde);
        }
        return -1;
    }
    else if(e->type == opp_binaire){
        ///(A V B) -> X: /a /b 
        if(e->u.opp_b.opp == OU){
            exppS * t = create_opp_unaire(NON, e->u.opp_b.op1);
            size = test(t,litterauxFind,size,sizeTab,monde);
            free(t);
            if(size < 0)
                return size;
            t = create_opp_unaire(NON, e->u.opp_b.op2);
            return test(t,litterauxFind,size,sizeTab,monde);
        }

        else if(e->u.opp_b.opp == ET){
            int temp;
            exppS * t = create_opp_unaire(NON, e->u.opp_b.op1);
            temp = test(t,litterauxFind,size,sizeTab,monde);
            
            if(temp < 0){
                free(t);
                t = create_opp_unaire(NON, e->u.opp_b.op2);
                return test(t,litterauxFind,size,sizeTab,monde);
            }
            return temp;
        }
        return -1;
    }
    else{
        exppS * t = create_opp_unaire(NON, e);
        int test = check_validiti(t,litterauxFind,size,sizeTab,monde);
        free(t);
        return test;
    }
}


int testUnaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
    if(e->u.opp_u.opp == NON){
        return rulesNON(e->u.opp_u.op1, litterauxFind,size,sizeTab,monde);
    }

    printf("Pas encore fait\n");
    return 0;
}

int testBinaire(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde){
    
    if(e->u.opp_b.opp == OU){
        int temp;

        temp = test(e->u.opp_b.op1,litterauxFind,size,sizeTab,monde);
        if(temp < 0){
            return test(e->u.opp_b.op2,litterauxFind,size,sizeTab,monde);
        }
        return temp;
    }

    if(e->u.opp_b.opp == ET){
        int temp;

        temp = test(e->u.opp_b.op1,litterauxFind,size,sizeTab,monde);
        if(temp < 0){
            return temp;
        }
        return test(e->u.opp_b.op2,litterauxFind,temp,sizeTab,monde);
    }


    printf("Pas encore fait\n");
    return 0;
}


// ///////////////// SUITE

// typedef struct branch { // arbre à 2 fils
//     exppS* e;
//     int monde;
//     struct branch* nexts[2];
// } branch;


// branch* create_branch() {
//     branch* b = (branch*) malloc(sizeof(branch));
//     b->nexts[0] = NULL;
//     b->nexts[1] = NULL;
//     b->monde = 0;
//     return b;
// }

// litteraux* create_litteraux() {
//     litteraux* b = (branch*) malloc(sizeof(litteraux));
//     b->nexts = NULL;
//     return b;
// }

// branch* goto_leaf(branch* b, int* current_path, int current_path_size) {
//     branch* leaf = b;
//     for (int i = 0; i < current_path_size; ++i)
//         if (leaf->nexts[current_path[i]]) leaf = leaf->nexts[current_path[i]];
//     return leaf;
// }

// // seul le noeud ou l'on branche aura pos = 1
// void add_next(branch* b, int pos/* 0 ou 1*/, int monde, exppS* e) {
//     b->nexts[pos] = (branch*) malloc(sizeof(branch));
//     b->nexts[pos]->monde = monde;
//     b->nexts[pos]->nexts[0] = NULL;
//     b->nexts[pos]->nexts[1] = NULL;
//     b->nexts[pos]->e = e;
// }


// // détection de l'opérateur
// int test_OPP(exppS* e, OPP o) {
//     switch(o) {
//         case NON:
//             return e->type == opp_unaire && e->u.opp_u.opp == NON;
//         case OU:
//             return e->type == opp_binaire && e->u.opp_b.opp == OU;
//         case ET:
//             return e->type == opp_binaire && e->u.opp_b.opp == ET;
//         case IMPLIQUE:
//             return e->type == opp_binaire && e->u.opp_b.opp == IMPLIQUE;
//         case CARRE:
//             return e->type == opp_unaire && e->u.opp_u.opp == CARRE;
//         case LOSANGE:
//             return e->type == opp_unaire && e->u.opp_u.opp == LOSANGE;
//     }
//     return -1;
// }

// // retourne 0 si tout vas bien ! !!
// // Un calcul bien lent ! :O

// int check_validity(branch* b) { // vérifie que l'on a pas trouver de contradiction    
//     return 0;
// }

// //  //a = a
// // retourne 0 si tout vas bien ! ! !
// int rule1(exppS* e, branch* b, int* current_path, int *current_path_size, int monde) {
//     if (test_OPP(e, NON)) {
//         exppS* son = e->u.opp_u.op1;
//         if (test_OPP(son, NON)) {
//             branch* l = goto_leaf(b, current_path, *current_path_size);
//             add_next(l, 0, monde, son->u.opp_u.op1);
//             current_path[*current_path_size] = 0;
//             (*current_path_size)++;
//             return check_validity(b);
//         }
//     }
//     return 0;
// }

// // a^b = a  b
// int rule2(exppS* e, branch* b, int* current_path, int *current_path_size, int monde) {
//     if (test_OPP(e, ET)) {
//         branch* l = goto_leaf(b, current_path, *current_path_size);
//         add_next(l, 0, monde, e->u.opp_b.op1);
//         current_path[*current_path_size] = 0;
//         (*current_path_size)++;
//         l = l->nexts[0];
//         add_next(l, 0, monde, e->u.opp_b.op2 );
//         current_path[*current_path_size] = 0;
//         (*current_path_size)++;
//         return check_validity(b);
//     }
//     return 0;
// }

// // /(AvB) = /a  /b
// int rule3(exppS* e, branch* b, int* current_path, int *current_path_size, int monde) {
//     if (test_OPP(e, NON)) {
//         exppS* son = e->u.opp_u.op1;
//         if (test_OPP(e, OU)) {
//             branch* l = goto_leaf(b, current_path, *current_path_size);
//             add_next(l, 0, monde, create_opp_unaire(NON, son->u.opp_b.op1));
//             current_path[*current_path_size] = 0;
//             (*current_path_size)++;
//             l = l->nexts[0];
//             add_next(l, 0, monde, create_opp_unaire(NON, son->u.opp_b.op2));
//             current_path[*current_path_size] = 0;
//             (*current_path_size)++;
//             return check_validity(b);
//         }
//     }
//     return 0;
// }

// // /(A->B) = a  /b
// int rule4(exppS* e, branch* b, int* current_path, int *current_path_size, int monde) {
//     if (test_OPP(e, NON)) {
//         exppS* son = e->u.opp_u.op1;
//         if (test_OPP(e, IMPLIQUE)) {
//             branch* l = goto_leaf(b, current_path, *current_path_size);
//             add_next(l, 0, monde, son->u.opp_b.op1);
//             current_path[*current_path_size] = 0;
//             (*current_path_size)++;
//             l = l->nexts[0];
//             add_next(l, 0, monde, create_opp_unaire(NON, son->u.opp_b.op2));
//             current_path[*current_path_size] = 0;
//             (*current_path_size)++;
//             return check_validity(b);
//         }
//     }
//     return 0;
// }

// /*      A SUIVRE       */
/* */ 




