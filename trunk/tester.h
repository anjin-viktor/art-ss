#ifndef TESTER_H_
#define TESTER_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include "manager.h"
#include "launcher.h"


#define TRUE_RET_VALUE 0
#define DEFAULT_NUMBER_OF_PROCESS 1
#define DEFAULT_TIME 2



/*
    return values:
	 0 - all true
	-1 - system error	
	-2 - incorrect used
	-3 - incorrect xml file
	-4 - error in open out_file (from tester_init_from_xml)
*/



typedef struct
{
    char 		*path;			/*path for the test program*/
    manager 		*pmanager;
    unsigned int 	n_launchers;		/*number of launchers*/
    launcher		*launchers;
    unsigned int	*times;			/*until the end of current launcher*/
    unsigned int	time;			/*all time*/
    char 		works;			/*works == 0 :not work else it's work*/
    FILE		*pfile;			/*file for debug messages*/
    action		act;
} tester;



int tester_init(tester *, const char *path, unsigned int n_launchers, unsigned int times, FILE *fp);
int tester_init_from_xml(tester *, const char *xml_file_name);
void tester_delete(tester *);
int tester_start(tester *);
int tester_stop(tester *);

/*My hidden function*/
unsigned int waiting_launcher(tester *);
void process_launcher(launcher *, FILE *);
void alarm_handler(int);
int init_tester_settings(tester *, xmlNodePtr node, xmlDocPtr);
int init_manager_settings(manager *, xmlNodePtr node, xmlDocPtr);
int init_general_settings(tester *, xmlNodePtr, xmlDocPtr);


#endif
