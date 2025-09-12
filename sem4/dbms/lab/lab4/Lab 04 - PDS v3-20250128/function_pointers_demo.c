#include <stdio.h>

void somefun1(char* s) { printf("somefun1 called with %s\n", s); }

void somefun2(char* s) { printf("somefun2 called with %s\n", s); }

void fpdemo(char* normalparam, void (*fpointer)(char*)); // in prototype decleration, we can remove the fpointer name

int somefun3(int x, int y) { return x + y; }

int main() {
    char* s = "Hello";

    printf("Calling fpdemo with somefun1 in main\n");
    fpdemo(s, somefun1);

    // printf("Calling fpdemo with somefun2 in main\n");
    // fpdemo(s, somefun2);

    // printf("Calling fpdemo with somefun3 in main\n");
    // somefun3(s, somefun3);

    return 0;
}

void fpdemo(char* normalparam, void (*fpointer)(char*)) {
    printf("Calling function pointer with param %s in fpdemo\n", normalparam);
    fpointer(normalparam);
}
