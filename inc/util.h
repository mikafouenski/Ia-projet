#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 1000

typedef enum OPP OPP;

enum OPP{
    NON, OU, ET, IMPLIQUE, CARRE, LOSANGE
};

#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c))



typedef struct exppS_ exppS;

struct exppS_ {
    enum {
        opp_unaire, opp_binaire, terme
    } type;
    union {
        struct {
            OPP opp;
            struct exppS_ * op1;
        } opp_u;
        struct {
            OPP opp;
            struct exppS_ * op1;
            struct exppS_ * op2;
        } opp_b;
        struct {
            char* c;
        } t;
    } u;
};

exppS* create_opp_unaire(OPP o, exppS* e);
exppS* create_opp_binaire(OPP o, exppS* e1, exppS* e2);
exppS* create_terme(char c);

void free_exppS(exppS* e);
void display_opp(OPP o);
void display_exppS(exppS* e);

typedef struct litteraux {
    exppS* e;
    int monde;
} litteraux;

typedef struct branch { // arbre à 2 fils
    exppS* e;
    int monde;
    struct branch* nexts[2];
} branch;

typedef void (*rule_base) (branch*);

typedef void (*syst) (branch*, int**, int*);

#endif
