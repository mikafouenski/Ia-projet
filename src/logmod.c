#include "modal.h"
#include "util.h"

void read_file(const char* name, char* buf) {
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }
    if (! fscanf(f, "%s", buf)) {
        perror("fscanf");
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

    int p = 0;
    exppS* r = expp(buf, &p, NULL);

    display_exppS(r);
    printf("\n");

    printf("%s\n", buf);

    free(buf);
    free_exppS(r);
    return 0;
}
