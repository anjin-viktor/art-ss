#ifndef _CREATE_LIB_H_
#define _CREATE_LIB_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "manager.h"

#define RETURN_VALUE 0


char create_lib_error[1024];

/*
create_lib return:
    -1: system error
    -2: error in description
*/

int create_lib(char *xml_file_name);
int create_function(xmlDocPtr doc, xmlNodePtr node, FILE *fp_h, FILE *fp_c, char **pstr); 
int read_general_settings(xmlDocPtr doc, xmlNodePtr node, int *res_not_process, int *act_if_over, char *act);
int write_head(FILE *fp_h, FILE *fp_c, int res_not_process, int act_if_over, const char *act);
int write_end(FILE *fp_h, char *str);


#endif
