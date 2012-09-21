#define _NOT_REDEF_
#include "catch_lib.h"

int catch_start(int argc, char **argv)
{
    extern manager catch_distributor;
	 atexit(catch_normal_exit);
    int i=0;
    manager_init(&catch_distributor);
    if(argc && (strcmp(argv[0], "complete_rejection") == 0 || strcmp(argv[0], "rejection") == 0 || strcmp(argv[0], "handling") == 0))
    {
        argc--;
        if(strcmp(argv[0], "complete_rejection") == 0)
            catch_distributor.act = complete_rejection;
        else if(strcmp(argv[0], "rejection") == 0)
            catch_distributor.act = rejection;
        else
            catch_distributor.act = handling;
        argv++;
    }
    else
        catch_distributor.act = handling;
    for(; i<argc; i+= 2)
        if(manager_add_str(&catch_distributor, argv[i], argv[i+1]) == -1)
	    return -1;
    return 0;
}
void catch_normal_exit(void)
{
    manager_delete(&catch_distributor);
    exit(0);
}
FILE * catch_fopen(const char * arg0, const char * arg1)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "fopen") == 0)
	return NULL;
    else
	return fopen(arg0, arg1);
}
int catch_open(const char * arg0, int arg1, ...)
{
    va_list ap;
    unsigned int s[16];
    int i=0;
    int size = 1;
    extern manager catch_distributor;
    va_start(ap, arg1);
    for(;i<size;i++ )
        s[i] = va_arg(ap, unsigned int);
    va_end(ap);
    if(manager_get_action(&catch_distributor, "open") == 0)
	return -1;
    else
	return open(arg0, arg1, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15]);
}
void * catch_malloc(size_t arg0)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "malloc") == 0)
	return NULL;
    else
	return malloc(arg0);
}
void * catch_calloc(size_t arg0, size_t arg1)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "calloc") == 0)
	return NULL;
    else
	return calloc(arg0, arg1);
}
void * catch_realloc(void * arg0, size_t arg1)
{
    extern manager catch_distributor;
    if(manager_get_action(&catch_distributor, "realloc") == 0)
	return NULL;
    else
	return realloc(arg0, arg1);
}
