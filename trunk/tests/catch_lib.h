#ifndef _CATCH_LIB_H_
#define _CATCH_LIB_H_

#include <stdarg.h>
#include <stdlib.h>
#ifndef _BIGBOOL_H_
#define _BIGBOOL_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BIGBOOL_BASE unsigned char

#define  BIGBOOL_SZ sizeof(BIGBOOL_BASE);

#define  BIGBOOL_SZ_BIT  (sizeof(BIGBOOL_BASE) * 8)

typedef struct
{
    BIGBOOL_BASE *pbool;
    unsigned int size_base;
    unsigned int size_bit;
} bigbool;

int bigbool_create(bigbool *pbig, int n);
void bigbool_init(bigbool *pbig);   /* create NULL bool */
void bigbool_delete(bigbool *pbig);
int bigbool_get(const bigbool *pbig, unsigned int n);
void bigbool_set(bigbool *pbig, unsigned int position, int value);
int bigbool_deikstre(bigbool *pbig);    /* next vector of Deikstre */
int bigbool_copy(bigbool *pdest, const bigbool *psrc);
int bigbool_create_str(bigbool *pbig, const char *str);
int bigbool_equal(const bigbool *pbig1, const bigbool *pbig2); /*return 0 if equal*/

#endif
#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RETURNED_END_BOOL 1


typedef struct
{
    bigbool              bool_describe;
    char                *name; 
    unsigned int       position;
    unsigned int     size;
    int                  action_after_end;
} resource;

void resource_init_null(resource *pres);
int resource_init(resource *pres, const char *name, const char *bool_str);
int resource_current_action(resource *pres);
void resource_delete(resource *pres);

/* return not zero if pres describe resource "name" */
int resource_is_my(resource *pres, const char *name);

int resource_copy(resource *prest, const resource *psrc);


#endif
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define RETURNED_RESOURCE_NOT_FIND 1




typedef enum {complete_rejection, rejection, handling, random_behavior, user_request} action;

/* complete_rejection: refusing all requests; rejection: refusing request,
if resource name not exist; handling: get result from bool vector*/

typedef struct
{
    resource                *presources;
    unsigned int  size;
    action          act;
    int                   returned_resource_not_find;
    unsigned int   next_changs;
} manager;

void manager_init(manager *pman);
int manager_add(manager *pman, const resource *pres);
int manager_add_str(manager *pman, const char *name, const char *str_bool);
int manager_get_action(const manager *pman, const char *resource_name);
void manager_delete(manager *pman);
void manager_next_state_bust(manager *pman);

/* return 1 if all resources are over else return zero */
int manager_all_end(const manager *pman);

#endif
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
