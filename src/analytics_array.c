#include "modal.h"
#include "util.h"
#include "read_formula.h"

#include <getopt.h>

void read_file(char* name, char* buf) {
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }
    if (fgets(buf, BUFFSIZE, f) == NULL) {
        perror("fgets");
        exit(1);
    }
    fclose(f);
}

void usage(char* exec_name) {
    fprintf(stderr, "Usage: %s <-k|-t|-4|-5> file_name\n", exec_name);
    fprintf(stderr, "\t-k : utilise le système K\n");
    fprintf(stderr, "\t-t : utilise le système T\n");
    fprintf(stderr, "\t-4 : utilise le système KT4\n");
    fprintf(stderr, "\t-5 : utilise le système KT45\n");
}

int main(int argc, char** argv) {
    if (argc - 1 != 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int opt, choise = 1;
    while ((opt = getopt(argc, argv, "kt45")) != -1) {
        switch (opt) {
            case 'k':
                choise = 1;
                break;
            case 't':
                choise = 2;
                break;
            case '4':
                choise = 3;
                break;
            case '5':
                choise = 4;
                break;
            default: /* '?' */
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char *buf = (char*) malloc (BUFFSIZE * sizeof(char));

    read_file(argv[optind], buf);
    printf("F :%s\n",buf);

    exppS* r = read_formula(buf);

    printf("Formule :  ");
    display_exppS(r);
    printf("\n");

    exppS* n = negate_exppS(r);

    printf("Négation :  ");
    display_exppS(n);
    printf("\n");

    int tautology = deriv(n, choise);
    if (tautology < 0) printf("TAUTOLOGIE ! ! !\n");
    else printf("PAS TAUTOLOGIE ! ! !\n");

    free(buf);
    free_exppS(n);
    return 0;
}
