#include <stdio.h>

#include "../tester.h"


void test1(void)
{
    tester t;
    tester_init(&t, "path", 1000, 5, stderr);
    tester_delete(&t);
    tester_delete(&t);
}


void test2(void)
{
    tester t;
    unsigned int i, n;
    char s1[1024], s2[1024];


    tester_init(&t, "print_argv", 1000, 2, stderr);

    fprintf(stdout, "size = ");
    fscanf(stdin, "%o", &n);

    for(i=0; i<n; i++)
    {
        fscanf(stdin, "%s", s1);
        fscanf(stdin, "%s", s2);

        manager_add_str(t.pmanager, s1, s2);    
    }

    n = tester_start(&t);
    fprintf(stderr, "result: %d", n);
}

void test3(void)
{
    unsigned int i;
    tester test;
    fprintf(stderr, "res: %d\n", tester_init_from_xml(&test, "test.xml"));
    fprintf(stdout, "path: %s\nn_launchers: %d\ntime: %d\nstream: %u\n", test.path, test.n_launchers, test.time, (unsigned int)test.pfile);

    switch(test.act)
    {
	case(complete_rejection):
	    fprintf(stdout, "complete_rejection\n");
	    break;
	
	case(rejection):
	    fprintf(stdout, "rejection\n");
	    break;
	case(handling):
	    fprintf(stdout, "handling\n");
	    break;
	case(random_behavior):
	    fprintf(stdout, "random_behavior\n");
	    break;
	
	case(user_request):
	    fprintf(stdout, "user_request\n");
	    break;
	
	default:
	    fprintf(stdout, "no action\n");
    };
    

    for(i=0; i<test.pmanager -> size; i++)
	fprintf(stdout, "%s\n", test.pmanager -> presources[i].name);
}


int main(void)
{
    test3();
    return 0;
}
