#ifndef _CATCH_LIB_H_
#define _CATCH_LIB_H_

#include <stdarg.h>
#include <stdlib.h>
#include "../manager.h"

manager catch_distributor;

int catch_start(int argc, char **argv);
void catch_normal_exit(void);
FILE * catch_fopen(const char * arg0, const char * arg1);
int catch_open(const char * arg0, int arg1, ...);
void * catch_malloc(size_t arg0);
void * catch_calloc(size_t arg0, size_t arg1);
void * catch_realloc(void * arg0, size_t arg1);

#ifndef _NOT_REDEF_
#define fopen(arg0, arg1) catch_fopen(arg0, arg1)
#define open(arg0, arg1, ...) catch_open(arg0, arg1,__VA_ARGS__)
#define malloc(arg0) catch_malloc(arg0)
#define calloc(arg0, arg1) catch_calloc(arg0, arg1)
#define realloc(arg0, arg1) catch_realloc(arg0, arg1)

#endif
#endif
