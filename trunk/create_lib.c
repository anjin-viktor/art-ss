#include "create_lib.h"

static const char *resource_src = "int resource_init(resource *pres, const char *res_name, const char *bool_str)\n{\n    int len = strlen(res_name) + 1;\n\n    pres -> action_after_end = RETURNED_END_BOOL;\n    pres -> name = (char *) malloc(len * sizeof(char)); \n    if(pres -> name == NULL)\n      return -1;\n    strcpy(pres -> name, res_name);\n    bigbool_init(& pres -> bool_describe);\n    if(bigbool_create_str(&pres -> bool_describe, bool_str) == -1)\n  return -1;\n    pres -> size = pres -> bool_describe.size_bit;\n    pres -> position = 0;\n    return 0;\n}\n\nint resource_current_action(resource *pres)\n{\n    int value;\n\n    if(pres -> position == pres -> size)\n        return pres -> action_after_end;\n\n    value = bigbool_get(&pres -> bool_describe, pres -> position);\n    pres -> position++;\n    return value;\n}\n\nvoid resource_delete(resource *pres)\n{\n    if(pres -> name)\n    {\n       free(pres -> name);\n   pres -> name = NULL;\n    }\n    bigbool_delete(&pres -> bool_describe);\n}\n\nint resource_is_my(resource *pres, const char *name)\n{\n    int result;\n\n    if(pres -> name == NULL)\n  return 0;\n\n    result = strcmp(pres -> name, name);\n    return !result;\n}\n\nint resource_copy(resource *pdest, const resource *psrc)\n{\n    if(pdest == psrc)\n      return 0;\n    if(bigbool_copy(&pdest -> bool_describe, &psrc -> bool_describe) == -1)\n        return -1;\n    pdest -> position = psrc -> position;\n    pdest -> size = psrc -> size;\n    if(psrc -> name == NULL)\n   return 0;\n    free(pdest -> name);\n    pdest -> name = (char *) malloc(sizeof(char) * (strlen(psrc -> name) + 1));\n    if(pdest == NULL)\n      return -1;\n    strcpy(pdest -> name, psrc -> name);\n    pdest -> action_after_end = psrc -> action_after_end;\n    return 0;\n}\n\nvoid resource_init_null(resource *pres)\n{\n    pres -> name = NULL;\n    pres -> action_after_end = RETURNED_END_BOOL;\n    bigbool_init(&pres -> bool_describe);\n}\n"; 
static const char *bigbool_src = "int bigbool_create(bigbool *pbig, int n)\n{\n    unsigned int i;\n\n    if(n < 0)\n     return -1;\n    if(pbig -> pbool)\n    {\n      free(pbig -> pbool);\n     pbig -> pbool = NULL;\n    }\n    pbig -> size_bit = n;\n    pbig -> size_base = (n + BIGBOOL_SZ_BIT - 1) / BIGBOOL_SZ_BIT;\n    if(n == 0)\n    {\n    pbig -> pbool = NULL;\n    return 0;\n    }\n    pbig -> pbool = (BIGBOOL_BASE*) malloc(n * sizeof(BIGBOOL_BASE));\n    if(pbig -> pbool == NULL)\n        return -1;\n    for(i=0; i < pbig -> size_base; i++)\n     pbig -> pbool[i] = 0;\n    return 0;\n}\n\nvoid bigbool_init(bigbool *pbig)\n{\n    pbig -> pbool = NULL;\n    pbig -> size_bit = 0;\n    pbig -> size_base = 0;\n}\n\nvoid bigbool_delete(bigbool *pbig)\n{\n    if(pbig -> pbool)\n    {\n       free(pbig -> pbool);\n  pbig -> pbool = NULL;\n pbig -> size_bit = 0;\n pbig -> size_base = 0;\n    }\n}\n\n\nint bigbool_get(const bigbool *pbig,  unsigned int n)\n{\n    unsigned int d = n / BIGBOOL_SZ_BIT;\n    unsigned int m = n % (BIGBOOL_SZ_BIT);\n    BIGBOOL_BASE k=1;\n    if(pbig -> pbool[d] & (k << m))\n    return 1;\n    return 0;\n}\n\nvoid bigbool_set(bigbool *pbig, unsigned int position, int value)\n{\n    int d, m;\n    BIGBOOL_BASE t;\n\n    value = !(!value);\n    \n    if(position >= pbig -> size_bit)\n    return;\n    d = position / BIGBOOL_SZ_BIT;\n    m = position % BIGBOOL_SZ_BIT;\n    t = (BIGBOOL_BASE) value;\n    t = t << m;\n    if(t)\n       pbig -> pbool[d] |= t;\n    else\n    {\n       t = ~(1 << m);\n        pbig -> pbool[d] &=t;\n    }\n}\n\n\nint bigbool_deikstre(bigbool *pbig)\n{\n    unsigned int i, j;\n\n    for(i=0; i<pbig->size_bit; i++)\n    {\n        if(bigbool_get(pbig, i) == 0)\n {\n         bigbool_set(pbig, i, 1);\n      j=0;\n          for(; j<i; j++)\n              bigbool_set(pbig, j, 0);\n          return 0;\n }\n    }\n    if(i == pbig -> size_bit)\n    {\n        if(pbig -> size_bit % BIGBOOL_SZ_BIT)\n {\n    pbig -> size_bit++;\n           bigbool_set(pbig, pbig -> size_bit-1, 1);\n             for(i=0; i < pbig -> size_bit-1; i++)\n             bigbool_set(pbig, i, 0);\n      }\nelse\n  {\n         BIGBOOL_BASE *tmp = (BIGBOOL_BASE*) malloc(sizeof(BIGBOOL_BASE) * (pbig -> size_base + 1));\n           if(tmp == NULL)\n           return -1;\n        for(i=0; i < pbig->size_base; i++)\n           tmp[i] = 0;\n       pbig -> size_bit++;\n           pbig -> size_base++;\n          if(pbig -> pbool)\n         free(pbig -> pbool);\n      pbig -> pbool = tmp;\n     bigbool_set(pbig, pbig -> size_bit-1, 1);\n }\n    }\n    return 0;\n}\n\nint bigbool_copy(bigbool *pdest, const bigbool *psrc)\n{\n    unsigned int i;\n    \n    bigbool_delete(pdest);\n    if(bigbool_create(pdest, psrc -> size_bit) == -1)\n     return -1;\n    \n    for(i=0; i<psrc -> size_bit; i++)\n    {\n        int value = bigbool_get(psrc, i);\n        bigbool_set(pdest, i, value);\n    }\n    return 0;\n}\n\nint bigbool_create_str(bigbool *pbig, const char *str)\n{\n    unsigned int len, i;\n\n    bigbool_delete(pbig);\n    len = strlen(str);\n    if(bigbool_create(pbig, len) == -1)\n      return -1;\n    \n    for(i=0; i<len; i++)\n    bigbool_set(pbig, i, str[i] - '0');\n    return 0;\n}\n\nint bigbool_equal(const bigbool *pbig1, const bigbool *pbig2)\n{\n    unsigned int i, n;\n    BIGBOOL_BASE mask;\n\n    if(pbig1 == pbig2)\n      return 0;\n\n    if(pbig1 -> size_bit != pbig2 -> size_bit)\n      return 1;\n\n    for(i=0; i<pbig1 -> size_base - 1; i++)\n      if(pbig1 -> pbool[i] != pbig2 -> pbool[i])\n        return 1;\n    \n    n = (pbig1 -> size_bit) % BIGBOOL_SZ_BIT;\n    mask = 1;\n    for(i=0; i<n; i++, mask <<= 1)\n    if((pbig1 -> pbool[pbig1 -> size_base - 1] & mask) != (pbig2 -> pbool[pbig2 -> size_base - 1] & mask))\n       return 1;\n\n    return 0;\n}\n"; 
static const char *manager_src = "void manager_init(manager *pman)\n{\n    pman -> returned_resource_not_find = RETURNED_RESOURCE_NOT_FIND;\n    pman -> presources = NULL;\n    pman -> size = 0;\n    pman -> next_changs = 0;\n}\n\n\nint manager_add(manager *pman, const resource *pres)\n{\n    resource *tmp;\n    unsigned int i;\n\n    if(pres == NULL)\n  return -1;\n\n    for(i=0; i < pman -> size; i++)\n        if(resource_is_my(&pman -> presources[i], pres -> name))\n          return -1;\n\n    tmp = (resource *) malloc(sizeof(resource) * (pman -> size + 1));\n    if(tmp == NULL)\n     return -1;\n    for(i=0; i<pman -> size + 1; i++)\n     resource_init_null(&tmp[i]);\n    for(i=0; i<pman -> size; i++)\n       if(resource_copy(&tmp[i], &pman -> presources[i]) == -1)\n     return -1;\n    if(resource_copy(&tmp[i], pres) == -1)\n    return -1;\n    if(pman -> presources)\n        free(pman -> presources);\n    pman -> presources = tmp;\n    pman -> size++;\n    return 0;\n}\n\n\nint manager_add_str(manager *pman, const char *name, const char *str_bool)\n{\n    resource *tmp;\n    unsigned int i;\n\n    if(name == NULL || str_bool == NULL)\n     return -1;\n\n    for(i=0; i < pman -> size; i++)\n     if(resource_is_my(&pman->presources[i], name))\n       return -1;\n\n    tmp = (resource *) malloc(sizeof(resource) * (pman -> size + 1));\n    if(tmp == NULL)\n  return -1;\n    for(i=0; i<pman -> size + 1; i++)\n     resource_init_null(&tmp[i]);\n    for(i=0; i<pman -> size; i++)\n  if(resource_copy(&tmp[i], &pman -> presources[i]) == -1)\n          return -1;\n    if(resource_init(&tmp[i], name, str_bool) == -1)\n     return -1;\n    if(pman -> presources)\n        free(pman -> presources);\n    pman -> presources = tmp;\n    pman -> size++;\n\n    return 0;\n}\n\n\nint manager_get_action(const manager *pman, const char *resource_name)\n{\n    unsigned int i;\n    char f;\n    int value;\n    \n    switch(pman -> act)\n    {\n case(complete_rejection):\n            value =  0;\n           break;\n\n  case(rejection):\n          value = pman -> returned_resource_not_find;\n           for(i=0; i<pman -> size; i++)\n       if(resource_is_my(&pman->presources[i], resource_name))\n                    value =  0;\n           break;\n\n\n        case(handling):\n           value = pman -> returned_resource_not_find;\n      for(i=0; i<pman -> size; i++)\n             if(resource_is_my(&pman->presources[i], resource_name))\n               {\n                 value = resource_current_action(&pman -> presources[i]);\n                 break;\n            }\n         break;\n\n  case(random_behavior):\n            value = rand() % 2;\n           break;\n\n  case(user_request):\n          for(f=1;f == 1;)\n      {\n         f=0;\n          fprintf(stderr, \"Resource %s\\nAction: \", resource_name);\n              fscanf(stdin, \"%d\", &value);\n             if(value != 0 && value != 1)\n          {\n                 f = 1;\n                fprintf(stderr, \"Error. Action is a 1 or 0\\n\");\n           }\n         }   \n    };\n\n    return value;\n}\n\nint manager_all_end(const manager *pman)\n{\n    unsigned int i=0;\n    for(i=0; i<pman -> size; i++)\n        if(pman -> presources[i].position < pman -> presources[i].size)\n      return 0;\n    return 1;\n}\n\nvoid manager_delete(manager *pman)\n{\n    unsigned int i;\n\n    if(pman -> presources == NULL)\n   return;\n\n    for(i=0; i<pman -> size; i++)\n     resource_delete(&pman -> presources[i]);\n    free(pman -> presources);\n}\n\nvoid manager_next_state_bust(manager *pman)\n{\n    if(pman -> size == 0)\n  return;\n\n    bigbool_deikstre(&pman -> presources[pman -> next_changs].bool_describe);\n    pman -> next_changs = (pman -> next_changs + 1) % pman -> size;\n}\n";
static const char *bigbool_h = "#ifndef _BIGBOOL_H_\n#define _BIGBOOL_H_\n\n#include <string.h>\n#include <stdlib.h>\n#include <stdio.h>\n\n#define BIGBOOL_BASE unsigned char\n\n#define  BIGBOOL_SZ sizeof(BIGBOOL_BASE);\n\n#define  BIGBOOL_SZ_BIT  (sizeof(BIGBOOL_BASE) * 8)\n\ntypedef struct\n{\n    BIGBOOL_BASE *pbool;\n    unsigned int size_base;\n    unsigned int size_bit;\n} bigbool;\n\nint bigbool_create(bigbool *pbig, int n);\nvoid bigbool_init(bigbool *pbig);   /* create NULL bool */\nvoid bigbool_delete(bigbool *pbig);\nint bigbool_get(const bigbool *pbig, unsigned int n);\nvoid bigbool_set(bigbool *pbig, unsigned int position, int value);\nint bigbool_deikstre(bigbool *pbig);    /* next vector of Deikstre */\nint bigbool_copy(bigbool *pdest, const bigbool *psrc);\nint bigbool_create_str(bigbool *pbig, const char *str);\nint bigbool_equal(const bigbool *pbig1, const bigbool *pbig2); /*return 0 if equal*/\n\n#endif\n";
static const char *resource_h = "#ifndef _RESOURCE_H_\n#define _RESOURCE_H_\n\n#include <string.h>\n#include <stdlib.h>\n#include <stdio.h>\n\n#define RETURNED_END_BOOL %d\n\n\ntypedef struct\n{\n    bigbool              bool_describe;\n    char                *name; \n    unsigned int       position;\n    unsigned int     size;\n    int                  action_after_end;\n} resource;\n\nvoid resource_init_null(resource *pres);\nint resource_init(resource *pres, const char *name, const char *bool_str);\nint resource_current_action(resource *pres);\nvoid resource_delete(resource *pres);\n\n/* return not zero if pres describe resource \"name\" */\nint resource_is_my(resource *pres, const char *name);\n\nint resource_copy(resource *prest, const resource *psrc);\n\n\n#endif\n";
static const char *manager_h = "#ifndef _MANAGER_H_\n#define _MANAGER_H_\n\n#include <stdlib.h>\n#include <stdio.h>\n#include <string.h>\n#include <libxml/xmlmemory.h>\n#include <libxml/parser.h>\n\n#define RETURNED_RESOURCE_NOT_FIND %d\n\n\n\n\ntypedef enum {complete_rejection, rejection, handling, random_behavior, user_request} action;\n\n/* complete_rejection: refusing all requests; rejection: refusing request,\nif resource name not exist; handling: get result from bool vector*/\n\ntypedef struct\n{\n    resource                *presources;\n    unsigned int  size;\n    action          act;\n    int                   returned_resource_not_find;\n    unsigned int   next_changs;\n} manager;\n\nvoid manager_init(manager *pman);\nint manager_add(manager *pman, const resource *pres);\nint manager_add_str(manager *pman, const char *name, const char *str_bool);\nint manager_get_action(const manager *pman, const char *resource_name);\nvoid manager_delete(manager *pman);\nvoid manager_next_state_bust(manager *pman);\n\n/* return 1 if all resources are over else return zero */\nint manager_all_end(const manager *pman);\n\n#endif\n";


int create_lib(char *xml_file_name)
{
    xmlDocPtr doc;
    xmlNodePtr node, tmp_node;
    FILE *fp_target_h, *fp_target_c;
    char *str_define;
    char f1, f2;
    int act_if_over = 1, res_not_process = 1;
    char act[64];

    create_lib_error[0] = 0;

    doc = xmlParseFile(xml_file_name);
    if(doc == NULL)
	return -1;

    node = xmlDocGetRootElement(doc);
    if(node == NULL)
    {
        sprintf(create_lib_error, "Xml file is empty");	
	xmlFreeDoc(doc);
    	return 0;
    }

    if(xmlStrcmp(node -> name, (const xmlChar *) "settings"))
    {
        sprintf(create_lib_error, "Xml file incorrect (not have tags \"test_settings\")");	
	xmlFreeDoc(doc);
	return -2;
    }

    node = node -> xmlChildrenNode;
    

    for(f1 = 1, f2 = 1, tmp_node = NULL;node && (f1 | f2); node = node -> next)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "functions"))
	{
	    tmp_node = node -> xmlChildrenNode;
	    f1 = 0;
	}

	if(!xmlStrcmp(node -> name, (const xmlChar *) "general_settings"))
	{
	    int ret_val = read_general_settings(doc, node -> xmlChildrenNode, &res_not_process, &act_if_over, act);

	    if(ret_val != 0)
	    {
	    	xmlFreeDoc(doc);
    		sprintf(create_lib_error, "Error in init general settings");	
		return ret_val;
	    }	    
	    f2 = 0;
	}
    }

    node = tmp_node;

    if(node == NULL)
    {
        sprintf(create_lib_error, "Xml file not have tags \"functions\"");	
	xmlFreeDoc(doc);
	return -2;    
    }


    fp_target_h = fopen("catch_lib.h", "w");

    if(fp_target_h == NULL)
    {
    	sprintf(create_lib_error, "System error");	
	xmlFreeDoc(doc);
	return -1;
    }

    fp_target_c = fopen("catch_lib.c", "w");

    if(fp_target_c == NULL)
    {
    	sprintf(create_lib_error, "System error");	
	xmlFreeDoc(doc);
	return -1;
    }

    if(write_head(fp_target_h, fp_target_c, res_not_process, act_if_over, act) == -1)
    {
	fclose(fp_target_h);
	fclose(fp_target_c);
	xmlFreeDoc(doc);
	return -1;
    }

    str_define = (char *) malloc(2 * sizeof(char));
    if(str_define == NULL)
    {
    	sprintf(create_lib_error, "System error");	
	fclose(fp_target_h);
	fclose(fp_target_c);
	xmlFreeDoc(doc);
	free(str_define);
	return -1;
    }

    str_define[0] = 0;

    while(node)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "function"))
	{
	    int res = create_function(doc, node, fp_target_h, fp_target_c, &str_define);
	    if(res == -1)
	    {
    		sprintf(create_lib_error, "System error");	
		fclose(fp_target_h);
		fclose(fp_target_c);
		xmlFreeDoc(doc);
		free(str_define);
		return -1;
	    }
	    if(res == -2)
	    {
        	sprintf(create_lib_error, "Incorrect usage (error in functions definitions)");	
		fclose(fp_target_h);
		fclose(fp_target_c);
		xmlFreeDoc(doc);
		free(str_define);
		return -2;
	    }
	}
	node = node -> next;
    }

    if(write_end(fp_target_h, str_define) == -1)
    {
    	sprintf(create_lib_error, "System error");	
	fclose(fp_target_h);
        fclose(fp_target_c);
        xmlFreeDoc(doc);
	free(str_define);
	return -1;
    }

    fclose(fp_target_h);
    fclose(fp_target_c);
    xmlFreeDoc(doc);
    free(str_define);

    sprintf(create_lib_error, "No error");
    return 0;
}

int create_function(xmlDocPtr doc, xmlNodePtr node, FILE *fp_h, FILE *fp_c, char **pstr)
{

    int n_parameters = 0;
    int list_arg = 0;
    int max_arg_size = 0;
    int i;
    int size = 0, tmp = 0;
    char *str_param = NULL, *tmp_str = NULL;

    xmlChar *name = NULL;
    xmlChar *parameters[2048];
    xmlChar *type = NULL;
    xmlChar *error_value = NULL;
    for(i=0; i<2048; i++)
	parameters[i] = NULL;

    if(xmlStrcmp(node -> name, (const xmlChar *) "function"))
	return 0;
    node = node -> xmlChildrenNode;


    while(node)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "name"))
	    name = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	
	if(!xmlStrcmp(node -> name, (const xmlChar *) "parameters"))
	{
	    xmlNodePtr child_node = node -> xmlChildrenNode;
	    while(child_node)
	    {
		if(!xmlStrcmp(child_node -> name, (const xmlChar *) "parameter"))
		{
		    parameters[n_parameters] = xmlNodeListGetString(doc, child_node -> xmlChildrenNode, 1);
		    n_parameters++;
		    if(!xmlStrcmp(parameters[n_parameters - 1], (const xmlChar *) "..."))
		    {
			list_arg = 1;
			n_parameters--;
		    }
		}
		child_node = child_node -> next;
	    }
	}

	if(!xmlStrcmp(node -> name, (const xmlChar *) "type"))
	    type = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	if(!xmlStrcmp(node -> name, (const xmlChar *) "error_value"))
	    error_value = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	
	if(!xmlStrcmp(node -> name, (const xmlChar *) "max_arg_size"))
	{
	    xmlChar *key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	    max_arg_size = atoi((const char *)key);
	    xmlFree(key);
	}
	node = node -> next;
    }

    if(!(error_value && type && name))
    {
	int i_param;

	sprintf(create_lib_error, "Error in description function %s", name);
	if(error_value)
	    xmlFree(error_value);
	if(type)
	    xmlFree(type);
	if(name)
	    xmlFree(name);

	for(i_param = 0; i_param < n_parameters; i_param++)
	    xmlFree(parameters[i_param]);
	if(list_arg)
	    xmlFree(parameters[i_param]);
	return -2;
    }

    if(list_arg && !max_arg_size)
    {
	int i_param;

	sprintf(create_lib_error, "Error in description function %s", name);
	xmlFree(error_value);
	xmlFree(type);
	xmlFree(name);
	for(i_param = 0; i_param < n_parameters; i_param++)
	    xmlFree(parameters[i_param]);
	if(list_arg)
	    xmlFree(parameters[i_param]);
	return -2;
    }

    fprintf(fp_h,"%s catch_%s(", type, name);
    fprintf(fp_c,"%s catch_%s(", type, name);

    for(i=0; i<n_parameters; i++)
    {
	if(i != 0)
	{
	    fprintf(fp_h, ", ");
	    fprintf(fp_c, ", ");
	}
	fprintf(fp_h, "%s arg%d", parameters[i], i);
	fprintf(fp_c, "%s arg%d", parameters[i], i);
    }
    if(list_arg)
    {
	fprintf(fp_h, ", ...");
	fprintf(fp_c, ", ...");
    }

    fprintf(fp_h, ");\n");
    fprintf(fp_c, ")\n{\n");
    if(list_arg == 0)
    {
	fprintf(fp_c,
	"    extern manager catch_distributor;\n"
	"    if(manager_get_action(&catch_distributor, \"%s\") == 0)\n" 
	"	return %s;\n"
	"    else\n"
	"	return %s("
	, name, error_value, name);

	for(i=0; i<n_parameters; i++)
	{
	    if(i != 0)
		fprintf(fp_c, ", ");
	    fprintf(fp_c, "arg%d", i);
	}
	fprintf(fp_c, ");\n}\n");
    }
    else
    {
	int size, i;
        if(max_arg_size % sizeof(int))
            size = max_arg_size / 4 + 1;
	else
	    size = max_arg_size / 4;

	fprintf(fp_c, 
	"    va_list ap;\n"
	"    unsigned int s[%d];\n"
	"    int i=0;\n"
	"    int size = %d;\n"
	"    extern manager catch_distributor;\n"
	"    va_start(ap, arg%d);\n"
	"    for(;i<size;i++ )\n"
	"        s[i] = va_arg(ap, unsigned int);\n"
	"    va_end(ap);\n"
	"    if(manager_get_action(&catch_distributor, \"%s\") == 0)\n" 
	"	return %s;\n"
	"    else\n"
	"	return %s("
	, size, n_parameters - 1, n_parameters - 1, name, error_value, name);
	for(i=0; i<n_parameters; i++)
	{
	    if(i != 0)
		fprintf(fp_c, ", ");
	    fprintf(fp_c, "arg%d", i);
	}
	for(i=0; i<size; i++)
	    fprintf(fp_c, ", s[%d]", i);
	fprintf(fp_c, ");\n}\n");
    }

    size  = 0;
    size += strlen("#define ") + 1;
    size += 2 * (strlen((const char *) name) + 3);
    tmp = n_parameters;
    for(i=0; tmp; tmp/=10, i++);
    
    size +=  2 * (n_parameters * (i + 7));
    if(list_arg)
	size += 25;

    str_param = (char *) malloc(size * sizeof(char));

    if(str_param == NULL)
    {
	int i_param;
	xmlFree(error_value);
	xmlFree(type);
	xmlFree(name);
	for(i_param = 0; i_param < n_parameters; i_param++)
	    xmlFree(parameters[i_param]);
	if(list_arg)
	    xmlFree(parameters[i_param]);
	sprintf(create_lib_error, "System error in create_function");
	return -1;
    }
    str_param[0] = 0;

    tmp_str = (char *) malloc((i + 7) * sizeof(char));
    if(tmp_str == NULL)
    {
	int i_param;

	xmlFree(error_value);
	xmlFree(type);
	xmlFree(name);
	free(str_param);
	for(i_param = 0; i_param < n_parameters; i_param++)
	    xmlFree(parameters[i_param]);
	if(list_arg)
	    xmlFree(parameters[i_param]);
	sprintf(create_lib_error, "System error in create_function");
	return -1;
    }
    for(i=0; i<n_parameters; i++)
    {
	if(i == 0)
	    sprintf(tmp_str, "arg%d", i);
	else
	    sprintf(tmp_str, ", arg%d", i);
        strcat(str_param,  tmp_str);    
    }

    free(tmp_str);
    size += strlen("catch_") + 3;

    if(list_arg)
    {
	int i_param;
	tmp_str = NULL;

	*pstr = (char *) (realloc(*pstr, (strlen(*pstr) + size + 1) * sizeof(char)));
	tmp_str = (char *) malloc (sizeof(char) * (size + 1));
	if(*pstr == NULL || tmp_str == NULL)
	{
    	    int i_param;

    	    xmlFree(error_value);
	    xmlFree(type);
	    xmlFree(name);
	    free(tmp_str);
	    for(i_param = 0; i_param < n_parameters; i_param++)
		xmlFree(parameters[i_param]);
	    if(list_arg)
		xmlFree(parameters[i_param]);
	    if(tmp_str)
		free(tmp_str);
	    sprintf(create_lib_error, "System error in create_function");
	    return -1;
	}

	sprintf(tmp_str, "#define %s(%s, ...) catch_%s(%s,__VA_ARGS__)\n", name, str_param, name, str_param);
	strcat(*pstr, tmp_str);
	free(tmp_str);
	free(str_param);
	for(i_param = 0; i_param < n_parameters; i_param++)
	    xmlFree(parameters[i_param]);
	xmlFree(parameters[i_param]);
	xmlFree(name);
	xmlFree(type);
	xmlFree(error_value);
        create_lib_error[0] = 0;
	return 0;
    }


    *pstr = (realloc(*pstr, (strlen(*pstr) + size + 1) * sizeof(char)));
    
    tmp_str = (char *) malloc((size + 1) * sizeof(char));
    if(tmp_str == NULL)
    {
	int i_param;

	xmlFree(error_value);
	xmlFree(type);
	xmlFree(name);
	free(str_param);
	for(i_param = 0; i_param < n_parameters; i_param++)
	    xmlFree(parameters[i_param]);
	if(list_arg)
	    xmlFree(parameters[i_param]);
        sprintf(create_lib_error, "System error in create_function");
	return -1;    
    }
    
    sprintf(tmp_str, "#define %s(%s) catch_%s(%s)\n", name, str_param, name, str_param);
    strcat(*pstr, tmp_str);


    xmlFree(error_value);
    xmlFree(type);
    xmlFree(name);
    free(str_param);
    free(tmp_str);
    for(i = 0; i < n_parameters; i++)
        xmlFree(parameters[i]);
    if(list_arg)
	xmlFree(parameters[i]);
    create_lib_error[0] = 0;
    return 0;
}



int write_head(FILE *fp_h, FILE *fp_c, int res_not_process, int act_if_over, const char *act)
{
    extern const char *bigbool_src, *resource_src, *manager_src;
    fprintf(fp_h,
    "#ifndef _CATCH_LIB_H_\n"
    "#define _CATCH_LIB_H_\n\n"
    "#include <stdarg.h>\n"
    "#include <stdlib.h>\n"
    );
  
    fputs(bigbool_h, fp_h);
    fprintf(fp_h, resource_h, act_if_over);
    fprintf(fp_h, manager_h, res_not_process);

    fprintf(fp_h,
    "int catch_start(int argc, char **argv);\n"
    "void catch_normal_exit(void);\n"
    );

/*
move "manager catch_distributor;\n\n" from fp_h to fp_c
*/
    fprintf(fp_c, 
    "#define _NOT_REDEF_\n"
    "#include \"catch_lib.h\"\n\n"
    "manager catch_distributor;\n\n"
    );

    fputs(bigbool_src, fp_c);
    fputs(resource_src, fp_c);
    fputs(manager_src, fp_c);
    
    fprintf(fp_c, "int catch_start(int argc, char **argv)\n"
    "{\n"
    "    extern manager catch_distributor;\n"
    "	 catch_distributor.act = %s;\n"
    "	 atexit(catch_normal_exit);\n"
    "    int i=0;\n"
    "    manager_init(&catch_distributor);\n"
    "    if(argc && (strcmp(argv[0], \"complete_rejection\") == 0 || strcmp(argv[0], \"rejection\") == 0 || strcmp(argv[0], \"random_behavior\") == 0 || strcmp(argv[0], \"user_request\") == 0 || strcmp(argv[0], \"handling\") == 0 ))\n"
    "    {\n"
    "        argc--;\n"
    "        if(strcmp(argv[0], \"complete_rejection\") == 0)\n"
    "            catch_distributor.act = complete_rejection;\n", 
    act);

    fprintf(fp_c, "        else if(strcmp(argv[0], \"rejection\") == 0)\n"
    "            catch_distributor.act = rejection;\n"
    "        else if(strcmp(argv[0], \"handling\") == 0 )\n"
    "            catch_distributor.act = handling;\n"
    "	     else if(strcmp(argv[0], \"random_behavior\") == 0)\n"
    "		 catch_distributor.act=random_behavior;\n"
    "	     else\n"
    "		 catch_distributor.act=user_request;\n"

    "        argv++;\n"
    "    }\n"
    "    for(; i<argc; i+= 2)\n"
    "        if(manager_add_str(&catch_distributor, argv[i], argv[i+1]) == -1)\n"
    "	    return -1;\n"
    "    return 0;\n"
    "}\n"
    "void catch_normal_exit(void)\n"
    "{\n"
    "    manager_delete(&catch_distributor);\n"
    "    exit(%d);\n"
    "}\n"
    , RETURN_VALUE);
    return 0;
}

int write_end(FILE *fp_h, char *str)
{
    fprintf(fp_h,
    "\n#ifndef _NOT_REDEF_\n"
    "%s\n"
    "#endif\n"
    "#endif\n"
    , str);
    return 0;
}

int read_general_settings(xmlDocPtr doc, xmlNodePtr node, int *res_not_process, int *act_is_over, char *act)
{
    xmlChar *key;

    *res_not_process = 1;
    *act_is_over = 1;
    strcpy(act, "handling");

    for(; node; node = node -> next)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "resource_not_processed"))
	{
            key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	    if(!xmlStrcmp(key, (const xmlChar *) "refusal"))
		*res_not_process = 0;
	    else
    		if(!xmlStrcmp(key, (const xmlChar *) "permission"))
		    *res_not_process = 1;
	}
	
	if(!xmlStrcmp(node -> name, (const xmlChar *) "action_if_over_vector"))
	{
            key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	    if(!xmlStrcmp(key, (const xmlChar *) "refusal"))
		*act_is_over = 0;
	    else
    		if(!xmlStrcmp(key, (const xmlChar *) "permission"))
		    *act_is_over = 1;
	}

	if(!xmlStrcmp(node -> name, (const xmlChar *) "behavior"))
	{
            key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);

	    if(!xmlStrcmp(key, (const xmlChar *) "complete_rejection"))
		strcpy(act, "complete_rejection");
	    else if(!xmlStrcmp(key, (const xmlChar *) "rejection"))
		strcpy(act, "rejection");
	    else if(!xmlStrcmp(key, (const xmlChar *) "handling"))
		strcpy(act, "handling");
	    else if(!xmlStrcmp(key, (const xmlChar *) "random_behavior"))
		strcpy(act, "random_behavior");
	    else if(!xmlStrcmp(key, (const xmlChar *) "user_request"))
		strcpy(act, "user_request");
	}
    }

    return 0;
}
