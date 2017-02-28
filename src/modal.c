#include "modal.h"

void expp_b(char* buf, int* p);
void expp(char* buf, int* p);
void opp(char* buf, int* p);
void opp_b(char* buf, int* p);

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

void eat_char(char* buf, int* p) {
    while(buf[*p] == ' ' || buf[*p] == '\n' || buf[*p] == '\t') (*p)++;
}

void conso(int* p) {
    (*p)++;
}

void expp_b(char* buf, int* p) {
    if (buf[*p] == '+') {
        conso(p);
        expp(buf, p);
    } else if (buf[*p] == '*') {
        conso(p);
        expp(buf, p);
    } else if (buf[*p] == '-') {
        conso(p);
        if (buf[*p] == '>') {
            conso(p);
            expp(buf, p);
        } else {
            printf("ERRR: attendu < \'>\' >\n");
            exit(1);
        }
    }
}

void expp(char* buf, int* p) {
    if (buf[*p] == '/') {
        conso(p);
        expp(buf, p);
    }  else if (buf[*p] == '[') {
        conso(p);
        if (buf[*p] == ']') {
            conso(p);
            expp(buf, p);
        } else {
            printf("ERRR: attendu < ] >\n");
            exit(1);
        }
    } else if (buf[*p] == '<') {
        conso(p);
        if (buf[*p] == '>') {
            conso(p);
            expp(buf, p);
        } else {
            printf("ERRR: attendu < \'<\' >\n");
            exit(1);
        }
    } else if (buf[*p] == '(') {
        conso(p);
        expp(buf, p);
    } else if (is_alpha(buf[*p])) {
        conso(p);
        expp_b(buf, p);
    } else {
        printf("ERRR: attendu < / | ( | char > \n");
        exit(1);
    }
}

int main(int argc, char const *argv[]) {
    if (argc - 1 != 1) {
        printf("usage: %s FileName\n", argv[0]);
        exit(1);
    }

    char *buf = (char*) malloc (BUFFSIZE * sizeof(char));

    read_file(argv[1], buf);

    int p = 0;
    expp(buf, &p);

    printf("%s\n", buf);

    free(buf);
    return 0;
}
