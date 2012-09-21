#include "catch.h"

#undef fopen//(path, mode) catch_fopen(path, mode)

void catchStart(int argc, char **argv)
{
    int n = argc - 1;
    for(int i=1; i<n; i+=2)
	Manager::getInstance().addResource(ResourceSettings(argv[i], Bigbool(argv[i+1])));
    FILE *fp = fopen(checkFileName.c_str(), "w");
    if(fp == NULL)
    {
	std::cerr << "catchStart: error. Can`t open check file " << checkFileName << endl;
	exit(1);
    }
    fclose(fp);
}

void catchStop()
{
    FILE *fp = fopen(checkFileName.c_str(), "w");
    if(fp == NULL)
    {
	std::cerr << "catchStart: error. Can`t open check file " << checkFileName << endl;
	exit(1);
    }
    fputs("true", fp);
    fclose(fp);
}


FILE *catch_fopen(const char *path, const char *mode)
{
    FILE *result;

    if(Manager::getInstance().getValue(fopenResourceName) == 0)
	result = NULL;
    else
	result = fopen(path, mode);
    return result;
}

#define fopen(path, mode) catch_fopen(path, mode)
