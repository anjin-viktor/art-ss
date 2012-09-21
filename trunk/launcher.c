#include "launcher.h"


int launcher_init(launcher *plaunch, const char *path)
{
    int len;

    plaunch -> force_end = 0;
    len = strlen(path);

    plaunch -> path = malloc(len + 1);
    if(plaunch -> path == NULL)
	return -1;

    strcpy(plaunch -> path, path);

    plaunch -> works = 0;
    plaunch -> process = -1;
    plaunch -> size = 0;
    plaunch -> res_names = NULL;
    plaunch -> res_bools = NULL;


    return 0;
}


void launcher_delete(launcher *plaunch)
{
    unsigned int i;

    launcher_stop(plaunch);

    if(plaunch -> path)
    {
	free(plaunch -> path);
	plaunch -> path = NULL;
    }

    for(i=0; i<plaunch -> size; i++)
    {
	free(plaunch -> res_names[i]);
	bigbool_delete(plaunch -> res_bools[i]);
	free(plaunch -> res_bools[i]);
    }
    
    if(plaunch -> size)
    {
	free(plaunch -> res_names);
	free(plaunch -> res_bools);
	plaunch -> res_names = NULL;
	plaunch -> res_bools = NULL;
    }
    plaunch -> size = 0;
}



int launcher_setting_from_manager(launcher *plaunch, const manager *pman)
{
    unsigned int *numbers;
    char **dels;
    unsigned int i, j, k;


    if((numbers = malloc(sizeof(unsigned int) * pman -> size)) == NULL)
	return -1;

    if((dels = malloc(sizeof(char *) * plaunch -> size)) == NULL)
    {
	free(numbers);
	return -1;
    }

    for(i=0, k=0; i<plaunch -> size; i++)
    {
	for(j=0; j<pman -> size && (strcmp(plaunch -> res_names[i], pman -> presources[j].name) != 0 || bigbool_equal(plaunch -> res_bools[i], &(pman -> presources[j].bool_describe))) != 0; j++);
	if(j == pman -> size)
	    dels[k++] = plaunch -> res_names[i];
    }

    for(i=0; i<k; i++)
	if(launcher_del_resource(plaunch, dels[i]) == -1)
	{
	    free(numbers);
	    free(dels);
	    return -1;
	}    

    free(dels);

    for(i=0, k=0; i<pman -> size; i++)
    {
	for(j=0; (j<plaunch -> size) && strcmp(pman -> presources[i].name, plaunch -> res_names[j]); j++);
    	if(j == plaunch -> size)
	    numbers[k++] = i;
    }


    for(i=0; i<k; i++)
    	if(launcher_add_resource(plaunch, pman -> presources[numbers[i]].name, &(pman -> presources[numbers[i]].bool_describe)) == -1)
	{
	    free(numbers);
	    return -1;
	}

    free(numbers);
    return -1;
}



int launcher_add_resource(launcher *plaunch, const char *name, const bigbool *pbool)
{
    char **tmp_names;
    bigbool **tmp_bools;
    unsigned int i;

    for(i=0; i<plaunch -> size; i++)
	if(strcmp(name, plaunch -> res_names[i]) == 0)
	    return -3;

    tmp_names = malloc(sizeof(char *) * (plaunch -> size + 1));
    if(tmp_names == NULL)
	return -1;

    tmp_bools = malloc(sizeof(bigbool *) * (plaunch -> size + 1));
    if(tmp_bools == NULL)
    {
	free(tmp_names);
	return -1;
    }

    for(i=0; i<plaunch->size; i++)
    {
	tmp_names[i] = plaunch -> res_names[i];
	tmp_bools[i] = plaunch -> res_bools[i];
    }

    tmp_names[i] = malloc(sizeof(char) * (strlen(name) + 1));
    if(tmp_names[i] == NULL)
    {
	free(tmp_names);
	free(tmp_bools);
	return -1;    
    }

    tmp_bools[i] = malloc(sizeof(bigbool));
    if(tmp_bools[i] == NULL)
    {
	free(tmp_names[i]);
	free(tmp_names);
	free(tmp_bools);
	return -1;    
    }

    strcpy(tmp_names[i], name);    
    
    bigbool_init(tmp_bools[i]);
    if(bigbool_copy(tmp_bools[i], pbool) < 0)
    {
	free(tmp_names[i]);
	free(tmp_names);
	free(tmp_bools);
	return -1;    
    }


    if(plaunch -> size)
    {
	free(plaunch -> res_names);
	free(plaunch -> res_bools);
    }

    plaunch -> res_names = tmp_names;
    plaunch -> res_bools = tmp_bools;
    plaunch -> size++;
    return 0;
}


int launcher_del_resource(launcher *plaunch, const char *name)
{
    unsigned int i, j;
    char **tmp_names;
    bigbool **tmp_bools;
    for(i=0; i<plaunch -> size; i++)
	if(strcmp(name, plaunch -> res_names[i]) == 0)
	    break;
    if(i == plaunch -> size)
	return 0;

    if(plaunch -> size == 1)
    {
	free(plaunch -> res_names[0]);
	bigbool_delete(plaunch -> res_bools[0]);

	free(plaunch -> res_names);
	free(plaunch -> res_bools);

	plaunch -> res_names = NULL;
	plaunch -> res_bools = NULL;
	plaunch -> size = 0;
	return 0;
    }

    tmp_names = malloc(sizeof(char *) * (plaunch -> size - 1));

    if(tmp_names == NULL)
	return -1;

    tmp_bools = malloc(sizeof(bigbool *) * (plaunch -> size - 1));
    if(tmp_bools == NULL)
    {
	free(tmp_names);
	return -1;
    }    

    for(i=0, j=0; i<plaunch->size; i++)
    {
	if(strcmp(name, plaunch-> res_names[i]) == 0)
	{
	    free(plaunch -> res_names[i]);
	    bigbool_delete(plaunch -> res_bools[i]);
	    free(plaunch -> res_bools[i]);
	    continue;
	}
	tmp_names[j] = plaunch -> res_names[i];
	tmp_bools[j] = plaunch -> res_bools[i];
	j++;
    }
    free(plaunch -> res_names);
    free(plaunch -> res_bools);

    plaunch -> res_names = tmp_names;
    plaunch -> res_bools = tmp_bools;
    (plaunch -> size)--;
    return 0;
}

int launcher_launch(launcher *plaunch, action act)
{
    extern char **environ;
    char **argv;
    unsigned int i=0, j;
    char action_str[32];

    if(plaunch -> works != 0)
	return -2;


    if((argv = malloc(sizeof(char *) * (plaunch -> size * 2 + 3))) == NULL)
	return -1;

    if((argv[0] = malloc(sizeof(char) * (strlen(plaunch -> path) + 1))) == NULL)
    {
	free(argv);
	return -1;
    }

    switch(act)
    {
        case(no_action):
	    break;

        case(complete_rejection):
	    strcpy(action_str, "complete_rejection");
    	    break;

    	case(rejection):
    	    strcpy(action_str, "rejection");
    	    break;

	case(handling):
    	    strcpy(action_str, "handling");
    	    break;

	case(random_behavior):
    	    strcpy(action_str, "random_behavior");
    	    break;

    	case(user_request):
    	    strcpy(action_str, "user_request");
    	    break;
    };

    if(act != no_action)
    {
	argv[1] = action_str;
	i = 1;
    }

    plaunch -> force_end = 0;
    plaunch -> works = 1;
    strcpy(argv[0], plaunch -> path);

    j=0;

    for(; i<(plaunch -> size) * 2; i+= 2)
    {
	if((argv[i + 1] = malloc(sizeof(char) * (strlen(plaunch -> res_names[i/2]) + 1))) == NULL
	|| (argv[i + 2]  = malloc(sizeof(char) * (plaunch -> res_bools[i/2] -> size_bit + 1))) == NULL)
	{
	    if(argv[i + 1] == NULL)
		free(argv[i + 1]);

	    for(j=0; j<i+1; j++)
		free(argv[j]);

	    free(argv);
	    plaunch -> works = 0;
	    return -1;
	}

	strcpy(argv[i + 1], plaunch -> res_names[i/2]);
	for(j=0; j<plaunch -> res_bools[i/2] -> size_bit; j++)
	    argv[i + 2][j] = bigbool_get(plaunch -> res_bools[i/2], j) + '0';
        argv[i + 2][j] = 0;
    }

    argv[i + 1] = NULL;

    switch((plaunch -> process = fork()))
    {
	case -1:
	{
	    for(j=0; j<(plaunch -> size * 2) + 1; j++)
		free(argv[j]);
	    free(argv);
	    plaunch -> works = 0;
	    return -1;
	}
	case 0:
	{
	    if(execve(plaunch -> path, argv, environ) == -1)
	    {
		exit(-1);
	    }
	}
	default:
	{
	    plaunch -> works = 1;
	}
    };

    for(j=0; j<(plaunch -> size * 2) + 1; j++)
	if(j != 1)
	    free(argv[j]);
	else
	{
	    if(act == no_action)
		free(argv[j]);
	}

    free(argv);
    return 0;
}

int launcher_stop(launcher *plaunch)
{
    if(plaunch -> works == 0) /*process not work*/
	return -2;

    if(kill(plaunch -> process, SIGKILL) == -1)
	return -1;

    plaunch -> force_end = 1;
    plaunch -> works = 0;
    plaunch -> process = -1;

    return 0;    
}
