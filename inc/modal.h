#ifndef modal
#define modal

#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 1000

enum {
    NON, OU, ET, IMPLIQUE, CARRE, LOSANGE
} OPP;

#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c))

typedef struct expp {
    enum {
        opp_unaire, opp_binaire, terme
    } t;
    union {
        struct {
            OPP opp;
            struct expp;
        } opp_unaire;
        struct {
            OPP opp;
            struct expp1;
            struct expp2;
        } opp_binaire;
        struct {
            char c;
        } terme;
    } u;
} expp;

#endif
