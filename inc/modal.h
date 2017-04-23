#ifndef __MODAL_H__
#define __MODAL_H__

#include "util.h"

exppS* negate_exppS(exppS* e);

int deriv(exppS* e, int choise);
void display_branch(branch* b, int indent);

#endif
