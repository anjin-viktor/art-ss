#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>



#include "resource.h"

#define RETURNED_RESOURCE_NOT_FIND 1




typedef enum {complete_rejection, rejection, handling, random_behavior, user_request, no_action} action;

/* 
complete_rejection: refusing all requests; 
rejection: refusing request, if resource name not exist; 
handling: get result from bool vector
random_behavior: return random value
user_request: request to conduct a user
*/

typedef struct
{
    resource 		*presources;
    unsigned int 	size;
    action 		act;
    int			returned_resource_not_find;
    unsigned int 	next_changs;
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
