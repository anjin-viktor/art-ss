#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string.h>
#include <stdlib.h>

#include "bigbool.h"

#define RETURNED_END_BOOL 1


typedef struct
{
    bigbool 		bool_describe;
    char 		*name; 
    unsigned int 	position;
    unsigned int 	size;
    int	 		action_after_end;
} resource;

void resource_init_null(resource *pres);
int resource_init(resource *pres, const char *name, const char *bool_str);
int resource_current_action(resource *pres);
void resource_delete(resource *pres);

/* return not zero if pres describe resource "name" */
int resource_is_my(resource *pres, const char *name);

int resource_copy(resource *prest, const resource *psrc);


#endif
