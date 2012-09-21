#include "resource.h"

int resource_init(resource *pres, const char *res_name, const char *bool_str)
{
    int len = strlen(res_name) + 1;

    pres -> action_after_end = RETURNED_END_BOOL;
    pres -> name = (char *) malloc(len * sizeof(char)); 
    if(pres -> name == NULL)
	return -1;
    strcpy(pres -> name, res_name);
    bigbool_init(& pres -> bool_describe);
    if(bigbool_create_str(&pres -> bool_describe, bool_str) == -1)
	return -1;
    pres -> size = pres -> bool_describe.size_bit;
    pres -> position = 0;
    return 0;
}

int resource_current_action(resource *pres)
{
    int value;

    if(pres -> position == pres -> size)
	return pres -> action_after_end;

    value = bigbool_get(&pres -> bool_describe, pres -> position);
    pres -> position++;
    return value;
}

void resource_delete(resource *pres)
{
    if(pres -> name)
    {
	free(pres -> name);
	pres -> name = NULL;
    }
    bigbool_delete(&pres -> bool_describe);
}

int resource_is_my(resource *pres, const char *name)
{
    int result;

    if(pres -> name == NULL)
	return 0;

    result = strcmp(pres -> name, name);
    return !result;
}

int resource_copy(resource *pdest, const resource *psrc)
{
    if(pdest == psrc)
	return 0;
    if(bigbool_copy(&pdest -> bool_describe, &psrc -> bool_describe) == -1)
	return -1;
    pdest -> position = psrc -> position;
    pdest -> size = psrc -> size;
    if(psrc -> name == NULL)
	return 0;
    free(pdest -> name);
    pdest -> name = (char *) malloc(sizeof(char) * (strlen(psrc -> name) + 1));
    if(pdest == NULL)
	return -1;
    strcpy(pdest -> name, psrc -> name);
    pdest -> action_after_end = psrc -> action_after_end;
    return 0;
}

void resource_init_null(resource *pres)
{
    pres -> name = NULL;
    pres -> action_after_end = RETURNED_END_BOOL;
    bigbool_init(&pres -> bool_describe);
}
