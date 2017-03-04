#ifndef modal
#define modal

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
    } t;
    union {
        struct {
            OPP opp;
            struct exppS_ * op1;
        } opp_unaire;
        struct {
            OPP opp;
            struct exppS_ * op1;
            struct exppS_ * op2;
        } opp_binaire;
        struct {
            char c;
        } terme;
    } u;
};

#endif
