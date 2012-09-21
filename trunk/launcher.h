#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include <string.h>
#include <stdlib.h>


#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

#include "bigbool.h"
#include "manager.h"



typedef struct 
{
    char		force_end;
    char		works;	/* != 0 - launcher busy */
    pid_t		process;

    char	 	*path;
    char 		**res_names;
    bigbool	 	**res_bools;
    unsigned int	size;
    
} launcher;

/*
return values:
     0 - true
    -1 - system error
    -2 - uncorrect used
    -3 - resource exist (launcher_add_resource)
*/


int launcher_init(launcher *, const char *path); /*set path*/
int launcher_setting_from_manager(launcher *, const manager *); /*copy description resources from struct manager*/
int launcher_add_resource(launcher *, const char *, const bigbool *);
int launcher_del_resource(launcher *, const char *);
void launcher_delete(launcher *);

int launcher_launch(launcher *, action); /*start process*/
int launcher_stop(launcher *);


#endif
