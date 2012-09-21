#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../bigbool.h"
#include "../resource.h"


int main(void)
{
    int n, i;
    char *str, *str_name;
    resource res, res1;

    str = (char *) malloc(1024);
    fprintf(stdout, "Bool:\n");
    i = fscanf(stdin, "%s", str);
    if(1 != i) {
        perror("fscanf()");
        exit(-1);
    }

    str_name = (char *) malloc(256);

    fprintf(stdout, "Name: ");
    i = fscanf(stdin, "%s", str_name);
    if(1 != i) {
        perror("fscanf()");
        exit(-1);
    }
    resource_init(&res, str_name, str);
    res.action_after_end = 0;

    n = strlen(str);

    resource_init_null(&res1);
    resource_copy(&res1, &res);
    do
    {
        fprintf(stdout, "Resource name: ");
        i = fscanf(stdin, "%s", str);
        if(1 != i) {
            perror("fscanf()");
            exit(-1);
        }
    }
    while(resource_is_my(&res1, str) == 0);
    fprintf(stdout, "Resource name: %s\n", res.name);

    for(i=0; i< n + 10; i++)
	fprintf(stdout, "%d", resource_current_action(&res1));
    fprintf(stdout, "\n");
    resource_delete(&res);
    resource_delete(&res1);
    return 0;
}
