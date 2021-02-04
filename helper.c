#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

// GET OPTION (CMD ARGS)
#include <getopt.h>

#include "helper.h"

int check(int exp, char *msg)
{
    if (exp <= SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}

int file_exists(char *filename)
{
    FILE* exist = fopen(filename, "r");
    if (exist != NULL){
        fclose(exist);
        return 1;
    } else { 
        return 0;
    }
}