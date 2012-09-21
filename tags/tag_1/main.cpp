#include "catch.h"
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{
    catchStart(argc, argv);
    
    char* str = new char[1024];
    for(int i=1; i<5; i++)
    {
	FILE *fp = fopen("test_file", "w");
	if(i % 2)
	    if(fp == NULL)
		continue;
	fgets(str, 1024, fp);
	fclose(fp);
    }
    delete [] str;
    catchStop();
    return 0;
}
