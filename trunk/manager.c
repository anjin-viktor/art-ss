#include "manager.h"

void manager_init(manager *pman)
{
    pman -> returned_resource_not_find = RETURNED_RESOURCE_NOT_FIND;
    pman -> presources = NULL;
    pman -> size = 0;
    pman -> next_changs = 0;
}


int manager_add(manager *pman, const resource *pres)
{
    resource *tmp;
    unsigned int i;

    if(pres == NULL)
	return -1;

    for(i=0; i < pman -> size; i++)
	if(resource_is_my(&pman -> presources[i], pres -> name))
	    return -1;

    tmp = (resource *) malloc(sizeof(resource) * (pman -> size + 1));
    if(tmp == NULL)
	return -1;
    for(i=0; i<pman -> size + 1; i++)
	resource_init_null(&tmp[i]);
    for(i=0; i<pman -> size; i++)
    	if(resource_copy(&tmp[i], &pman -> presources[i]) == -1)
	    return -1;
    if(resource_copy(&tmp[i], pres) == -1)
	return -1;
    if(pman -> presources)
	free(pman -> presources);
    pman -> presources = tmp;
    pman -> size++;
    return 0;
}


int manager_add_str(manager *pman, const char *name, const char *str_bool)
{
    resource *tmp;
    unsigned int i;

    if(name == NULL || str_bool == NULL)
	return -1;

    for(i=0; i < pman -> size; i++)
	if(resource_is_my(&pman->presources[i], name))
	    return -1;

    tmp = (resource *) malloc(sizeof(resource) * (pman -> size + 1));
    if(tmp == NULL)
	return -1;
    for(i=0; i<pman -> size + 1; i++)
    	resource_init_null(&tmp[i]);
    for(i=0; i<pman -> size; i++)
	if(resource_copy(&tmp[i], &pman -> presources[i]) == -1)
	    return -1;
    if(resource_init(&tmp[i], name, str_bool) == -1)
	return -1;
    if(pman -> presources)
	free(pman -> presources);
    pman -> presources = tmp;
    pman -> size++;

    return 0;
}


int manager_get_action(const manager *pman, const char *resource_name)
{
    unsigned int i;
    char f;
    int value;
    
    switch(pman -> act)
    {
	case(complete_rejection):
	    value =  0;
	    break;

	case(rejection):
	    value = pman -> returned_resource_not_find;
	    for(i=0; i<pman -> size; i++)
		if(resource_is_my(&pman->presources[i], resource_name))
		    value =  0;
	    break;


	case(handling):
	    value = pman -> returned_resource_not_find;
	    for(i=0; i<pman -> size; i++)
		if(resource_is_my(&pman->presources[i], resource_name))
		{
		    value = resource_current_action(&pman -> presources[i]);
		    break;
		}
	    break;

	case(random_behavior):
	    value = rand() % 2;
	    break;

	case(user_request):
	    for(f=1;f == 1;)
	    {
		f=0;
	        fprintf(stderr, "Resource %s\nAction: ", resource_name);
		fscanf(stdin, "%d", &value);
	        if(value != 0 && value != 1)
		{
		    f = 1;
		    fprintf(stderr, "Error. Action is a 1 or 0\n");
	        }
	    }	
	    break;

	case(no_action):
	    value = 1;
	    break;

    };

    return value;
}

int manager_all_end(const manager *pman)
{
    unsigned int i=0;
    for(i=0; i<pman -> size; i++)
	if(pman -> presources[i].position < pman -> presources[i].size)
	    return 0;
    return 1;
}

void manager_delete(manager *pman)
{
    unsigned int i;

    if(pman -> presources == NULL)
	return;

    for(i=0; i<pman -> size; i++)
	resource_delete(&pman -> presources[i]);
    free(pman -> presources);
}

void manager_next_state_bust(manager *pman)
{
    if(pman -> size == 0)
	return;

    bigbool_deikstre(&pman -> presources[pman -> next_changs].bool_describe);
    pman -> next_changs = (pman -> next_changs + 1) % pman -> size;
}
