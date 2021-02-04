#include <stdio.h>
#include <stdlib.h>
#include "./server.h"


int main(int argc, char *argv[]) {
    printf("Main\n");

    start_server(THREADS);

    return 0;
}