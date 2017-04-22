#include "modal.h"
#include "util.h"
#include "read_formula.h"

void read_file(const char* name, char* buf) {
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

int main(int argc, char const *argv[]) {
    if (argc - 1 != 1) {
        printf("usage: %s FileName\n", argv[0]);
        exit(1);
    }

    char *buf = (char*) malloc (BUFFSIZE * sizeof(char));

    read_file(argv[1], buf);
    printf("F :%s\n",buf);

    exppS* r = read_formula(buf);

    printf("Formule :  ");
    display_exppS(r);
    printf("\n");

    exppS* n = negate_exppS(r);

    printf("Négation :  ");
    display_exppS(n);
    printf("\n");

    int tautologie = deriv(n);
    if (tautologie < 0) printf("TAUTOLOGIE ! ! !\n");
    else printf("PAS TAUTOLOGIE ! ! !\n");

    free(buf);
    free_exppS(n);
    return 0;
}
