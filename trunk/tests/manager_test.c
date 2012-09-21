#include <stdio.h>

#include "manager.h"


void test1(void)
{
    manager man;
    unsigned int i, n, j;
    char s1[1024], s2[1024];

    manager_init(&man);
    fprintf(stdout, "size = ");
    fscanf(stdin, "%o", &n);

    for(i=0; i<n; i++)
    {
        fscanf(stdin, "%s", s1);
        fscanf(stdin, "%s", s2);

	manager_add_str(&man, s1, s2);    
    }

    for(;;)
    {
	for(i=0; i<man.size; i++)
	{
	    fprintf(stdout, "%s: ", man.presources[i].name);
	    for(j=0; j<man.presources[i].bool_describe.size_bit; j++)
		fprintf(stdout, "%o", bigbool_get(&man.presources[i].bool_describe, j));
	    fprintf(stdout, "\n");	
	}

	fgets(s1, 1024, stdin);
	manager_next_state_bust(&man);
    }
}




int main(int argc, char **argv)
{
    manager man;
    resource res;
    char *str1, *str2;
    int retval;

/*
*/
    test1();

    if(argc < 2)
    {
	fprintf(stderr, "Arg: mode (complete_rejection, rejection, handling)\n");
	return 0;
    }

    manager_init(&man);
    if(strcmp(argv[1], "complete_rejection") == 0)
	man.act  = complete_rejection;
    else if(strcmp(argv[1], "rejection") == 0)
	man.act = rejection;
    else
	man.act = handling;

    str1 = (char *) malloc(1024);
    str2 = (char *) malloc(1024);
    for(;;)
    {
	fprintf(stdout, "Resource name: ");
        retval = fscanf(stdin, "%s", str1);
	if(retval != 1) {
        perror("fscanf()");
        exit(-1);
    }

	if(strcmp(str1, "end") == 0)
	    break;
	fprintf(stdout, "Vector: ");
    retval = fscanf(stdin, "%s", str2);
    if(retval != 1) {
        perror("fscanf()");
        exit(-1);
    }

	resource_init(&res, str1, str2);
	manager_add(&man, &res);
	fprintf(stdout, "\n");
    }
/*    fprintf(stderr, "start"); */

    for(;;)
    {
    int value;
	fprintf(stdout, "Resource name: ");
    retval = fscanf(stdin, "%s", str1);
    if(retval != 1) {
        perror("fscanf()");
        exit(-1);
    }

	value = manager_get_action(&man, str1);
	fprintf(stdout, "%d\n", value);
    }

    manager_delete(&man);
    return 0;
}
