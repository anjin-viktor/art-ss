#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../bigbool.h"



void equal_test(void)
{
    bigbool obj1, obj2;
    char s[1024];
    fgets(s, 1024, stdin);
    s[strlen(s)] = '\0';
    
    bigbool_init(&obj1);
    bigbool_init(&obj2);

    bigbool_create_str(&obj1, s);

    fgets(s, 1024, stdin);
    s[strlen(s)] = '\0';
    bigbool_create_str(&obj2, s);

    fprintf(stdout, "%d\n", bigbool_equal(&obj1, &obj2));
    exit(0);
}




int main(void)
{
    bigbool obj1, obj2, obj3;
    int i, j, n;
    char *a;
    char *str;

    equal_test();

    srand(time(NULL));
    bigbool_init(&obj1);
    bigbool_init(&obj2);
    bigbool_init(&obj3);

    for(j=0;;j++)
    {
	n = rand() % 10000 + 1;
	bigbool_create(&obj1, n);
	a = (char*) malloc(n);
	str = (char*) malloc(n + 1);
	
	for(i=0; i<n; i++)
	{
	    a[i] = rand() % 2;
	    str[i] = a[i] + '0';
	    bigbool_set(&obj1, i, a[i]);
	}
	
	str[i] = '\0';
	bigbool_create_str(&obj2, str);

	for(i=0; i<n; i++)
	{
	    if(a[i] != bigbool_get(&obj1, i))
	    {
		fprintf(stderr, "Error. Bigbool incorect\n");
		return 1;
	    }
	    if(str[i] - '0' != bigbool_get(&obj2, i))
	    {
		fprintf(stderr, "Error. Create for str incorrect\n");
		return 2;
	    }
	}

	bigbool_copy(&obj3, &obj2);
	bigbool_delete(&obj1);
	bigbool_delete(&obj2);
	for(i=0; i<n; i++)
	{
	    if(a[i] != bigbool_get(&obj3, i))
	    {
		fprintf(stderr, "Error. Copy bigbool incorect\n");
		return 2;
	    }
	}
	free(a);
	free(str);
	bigbool_delete(&obj3);
	fprintf(stderr, "%d: %d\n", j,n);
    }
/*    for(;;)
    {
	for(i=0; i<obj.size_bit; i++)
	    fprintf(stderr, "%d", bigbool_get(&obj, i));
	fprintf(stderr, "\n");
	getc(stdin);
	bigbool_deikstre(&obj);
    }
    bigbool_delete(&obj);*/
    return 0;
}
