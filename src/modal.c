#include "modal.h"
#include <string.h>

exppS* negate_exppS(exppS* e) {
    exppS* n = create_opp_unaire(NON, e);
    return n;
}

void add_in_branch(branch* b, int indi, branch* add) {
    branch * elm = b;
    while(elm->nexts[indi])
        elm = elm->nexts[indi];
    elm->nexts[indi]= add;

}

branch* create_branch(exppS* e, int world) {
    branch* b = (branch*) malloc(sizeof(branch));
    b->nexts[0] = NULL;
    b->nexts[1] = NULL;
    b->e = e;
    b->world = world;
    return b;
}

void display_branch(branch* b, int indent) {
    for (int i = 0; i < indent; ++i) printf("  ");
    if (b->e != NULL) {
        display_exppS(b->e);
        printf("  M:%d\n", b->world);
    }
    if (b->nexts[0])
        display_branch(b->nexts[0], indent + 1);
    if (b->nexts[1])
        display_branch(b->nexts[1], indent + 1);
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

int addLiteral(litteraux *litterauxFind, exppS* e, int world, int size) {
    litterauxFind[size].e = e;
    litterauxFind[size].world = world;
    ++size;
    return size;
}

void display_litteraux(litteraux* litterauxFind, int size) {
    printf("déjas trouver %d\n", size);
    for (int i = 0; i < size; ++i) {
        if(litterauxFind[i].e->type == terme)
            printf("%s-", litterauxFind[i].e->u.t.c);
        else
            printf("non(%s)-", litterauxFind[i].e->u.opp_u.op1->u.t.c);
    }
    printf("\n");
}

int check_validity(exppS* e, litteraux *litterauxFind, int size, int world) {
    //On ajoute un terme positif on regarde si un négatif contredi
    if(e->type == terme) {
        for (int i = 0; i < size; ++i)
            if(litterauxFind[i].world == world && litterauxFind[i].e->type == opp_unaire)
                if(strcmp(litterauxFind[i].e->u.opp_u.op1->u.t.c, e->u.t.c) == 0)
                    return -1;
    } else { //On ajoute un NON
        char* c = e->u.opp_u.op1->u.t.c;
        for (int i = 0; i < size; ++i) {
            if(litterauxFind[i].world == world && litterauxFind[i].e->type == terme) {
                printf("%s", litterauxFind[i].e->u.t.c);
                if(strcmp(c, litterauxFind[i].e->u.t.c) == 0)
                    return -1;
            }
        }
    }
    return addLiteral(litterauxFind, e, world, size);
}

// //a = a
void rule_base_1(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, NON)) { // j'ai trouver //a
        exppS* t = (exppS*) malloc(sizeof(exppS));
        memcpy(t, b->e->u.opp_u.op1->u.opp_u.op1, sizeof(exppS));
        add_in_branch(b, 0, create_branch(t, b->world));
    }
}

// a^b = a  b
void rule_base_2(branch* b) {
    if (test_opp(b->e, ET)) { // j'ai trouver a^b
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        add_in_branch(b, 0, create_branch(op1, b->world));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        add_in_branch(b, 0, create_branch(op2, b->world));
    }
}

// /(AvB) = /a  /b
void rule_base_3(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, OU)) { // /(AvB)
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        add_in_branch(b, 0, create_branch(non_op1, b->world));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        add_in_branch(b, 0, create_branch(non_op2, b->world));
    }
}

// /(A->B) = a  /b
void rule_base_4(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, IMPLIQUE)) { // /(A->B)
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        add_in_branch(b, 0, create_branch(op1, b->world));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        add_in_branch(b, 0, create_branch(non_op2, b->world));
    }
}

// AvB = a | b     -> on branche
void rule_base_5(branch* b) {
    if (test_opp(b->e, OU)) { // j'ai trouver AvB
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        add_in_branch(b, 0, create_branch(op1, b->world));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        add_in_branch(b, 1, create_branch(op2, b->world));
    }
}

// /(A^B) = /a | /b     -> on branche
void rule_base_6(branch* b) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, ET)) { // j'ai trouver /(a^b)
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_u.op1->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        add_in_branch(b, 0, create_branch(non_op1, b->world));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_u.op1->u.opp_b.op2, sizeof(exppS));
        exppS* non_op2 = negate_exppS(op2);
        add_in_branch(b, 1, create_branch(non_op2, b->world));
    }
}

int ruleConflit(branch* b, litteraux* litterauxFind, int size) {
    if(is_terme(b->e) || is_non_terme(b->e))
        return check_validity(b->e, litterauxFind, size, b->world);
    return 0;
}

// A->B = /a | b     -> on branche
void rule_base_7(branch* b) {
    if (test_opp(b->e, IMPLIQUE)) { // j'ai trouver A->B
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        add_in_branch(b, 0, create_branch(non_op1, b->world));
        exppS* op2 = (exppS*) malloc(sizeof(exppS));
        memcpy(op2, b->e->u.opp_b.op2, sizeof(exppS));
        add_in_branch(b, 1, create_branch(op2, b->world));
    }
}

// <>A = /[]/a
void rule_base_8(branch* b) {
    if (test_opp(b->e, LOSANGE)) { // j'ai trouver A->B
        exppS* op1 = (exppS*) malloc(sizeof(exppS));
        memcpy(op1, b->e->u.opp_b.op1, sizeof(exppS));
        exppS* non_op1 = negate_exppS(op1);
        exppS* carre = create_opp_unaire(CARRE, non_op1);
        exppS* non_carre = negate_exppS(carre);
        add_in_branch(b, 0, create_branch(non_carre, b->world));
    }
}

// [][]A = []A
void rule_4_1(branch *b){
    if(test_opp(b->e,CARRE) && test_opp(b->e->u.opp_u.op1, CARRE)){
        add_in_branch(b, 0, create_branch(b->e->u.opp_u.op1, b->world));
    }
}

void add_in_all_branch(branch* b, branch* new_branch) {
    if (b->nexts[0] == NULL) b->nexts[0] = new_branch;
    else add_in_all_branch(b->nexts[0], new_branch);
    if (b->nexts[1]) add_in_all_branch(b->nexts[1], new_branch);
}

void system_k_1(branch* b, int** worldFind, int* nbWorld) {
    if(test_opp(b->e, CARRE)) {
        add_in_all_branch(b, create_branch(b->e, b->world));
        for (int i = 0; i < *nbWorld + 1; ++i)
            if(worldFind[b->world][i] == 1)
                add_in_branch(b, 0, create_branch(b->e->u.opp_u.op1, worldFind[b->world][i]));
    }
}

void replicWorld(int ** worldFind,int* nbWorld,int world){
    for (int i = 0; i < world; ++i)
    {
        if(worldFind[i][world] == 1){
            worldFind[i][*nbWorld] = 1;
        }
    }
}

void system_k_2(branch *b, int** worldFind, int* nbWorld) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, CARRE)) {
        (*nbWorld)++;
        replicWorld(worldFind,nbWorld,b->world);
        worldFind[b->world][*nbWorld] = 1;

        exppS* non_op1 = negate_exppS((b->e->u.opp_u.op1->u.opp_u.op1));
        add_in_branch(b, 0, create_branch(non_op1, *nbWorld));
    }
}


void system_4_2(branch *b, int** worldFind, int* nbWorld) {
    if (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, CARRE)) {
        (*nbWorld)++;

        worldFind[b->world][*nbWorld] = 1;

        exppS* non_op1 = negate_exppS((b->e->u.opp_u.op1->u.opp_u.op1));
        add_in_branch(b, 0, create_branch(non_op1, *nbWorld));
    }
}

int isBranch(branch* b) {
    return (test_opp(b->e, NON) && test_opp(b->e->u.opp_u.op1, ET)) ||
        (test_opp(b->e, IMPLIQUE)) ||
        (test_opp(b->e, OU));
}

int allBranch(branch* b) {
    branch *tmp = b;
    while(tmp) {
        if(!isBranch(b) && !test_opp(b->e, CARRE)) {
            return 0;
        }
        tmp = tmp->nexts[0];
    }
    return 1;
}

// retourne 1 si tautologie
int deriv_back(branch* b, rule_base* base, int base_size, syst* extension, int extension_size, litteraux* litterauxFind, int size, int** worldFind, int* nbWorld) {
    while(b!= NULL && isBranch(b)) {
        if(!allBranch(b->nexts[0])) add_in_branch(b, 0, create_branch(b->e, b->world));
        else break;
        b = b->nexts[0];
    }

    for (int i = 0; i < base_size; ++i) (*base[i])(b); // appel de toutes les règles du système base

    int temp = ruleConflit(b, litterauxFind, size);
    if (temp < 0) return -1;
    if(temp > 0) size = temp;

    for (int i = 0; i < extension_size; ++i) (*extension[i])(b, worldFind, nbWorld); // appel des regles du systeme d'extension

    int r = 0;
    if (b->nexts[0] != NULL)
        r = deriv_back(b->nexts[0], base, base_size, extension, extension_size, litterauxFind, size, worldFind, nbWorld);
    //Branche valide
    if(r > 0) {
        printf("Branche juste trouver\n");
        return 1;
    }
    if (b->nexts[1] != NULL)
        r = deriv_back(b->nexts[1], base, base_size, extension, extension_size, litterauxFind, size, worldFind, nbWorld);
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

    litteraux* litterauxFind = (litteraux*) malloc(sizeof(litteraux) * 100);
    int system_base_size = 8;
    rule_base system_base[8] = {rule_base_1, rule_base_2, rule_base_3,
                                rule_base_4, rule_base_5, rule_base_6,
                                rule_base_7, rule_base_8};

    int system_k_size = 2;
    syst system_k[2] = {system_k_1, system_k_2};

    int** worldFind = (int**) malloc(sizeof(int*) * 100);
    for (int i = 0; i < 100; ++i)
        worldFind[i] = (int*) malloc(sizeof(int)*100);

    int nbWorld = 0;
    int r = deriv_back(head, system_base, system_base_size, system_k, system_k_size, litterauxFind, 0, worldFind, &nbWorld);

    display_branch(head, 0);
    free_branch(head);
    free(litterauxFind);
    return r;
}
