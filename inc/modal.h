#ifndef modal
#define modal

#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 1000

// #define enum OPP {
//     NON, OU, ET, IMPLIQUE, CARRE, LOSANGE
// };

#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c))

#endif
