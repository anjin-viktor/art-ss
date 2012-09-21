#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "../catch_lib.h"

int main(int argc, char **argv)
{
    int i;
    char *p = 0;

    catch_start(--argc, ++argv);

    srand(time(NULL));
    fprintf(stderr, "Hello!\n");
    for(i=0; i<argc; i++)
	fprintf(stderr, "%s\n", argv[i]);    


    fopen("/dev/random", "r");
    malloc(5);

    if((rand() % 10) == 0)
	strncpy(p, "hello", 5);


    sleep(4);
    fprintf(stderr, "end\n");
    return 0;
}
