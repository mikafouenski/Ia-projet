#ifndef __MODAL_H__
#define __MODAL_H__

#include "util.h"

void conso(char* buf, int* p);

exppS* opp_impl(char* buf, int* p, exppS* h);
exppS* opp_ou(char* buf, int* p, exppS* h);
exppS* opp_et(char* buf, int* p, exppS* h);
exppS* termeInf(char* buf, int* p);
exppS* unaire(char* buf, int* p, OPP o);
exppS* expp(char* buf, int* p, exppS *herite);

exppS* negate_exppS(exppS* e);

int test(exppS* e, litteraux *litterauxFind, int size, int sizeTab, int monde);

int deriv(exppS* e);
void display_branch(branch* b, int indent);

#endif
