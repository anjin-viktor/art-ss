#include "tester.h"

tester *tester_pointer;

int tester_init(tester *ptest, const char *path, unsigned int n, unsigned int time, FILE *fp)
{
    unsigned int i;
    int j;

    extern tester *tester_pointer;

    ptest -> pfile = fp;

    ptest -> n_launchers = n;
    ptest -> time = time;

    ptest -> pmanager = NULL;
    ptest -> launchers = NULL;
    ptest -> times = NULL;
    ptest -> works = 0;
    ptest -> act = no_action;

    if((ptest -> path = malloc((strlen(path) + 1) * sizeof(char))) == NULL)
	return -1;

    strcpy(ptest -> path, path);


    if((ptest -> pmanager = malloc(sizeof(manager))) == NULL)
    {
	free(ptest -> path);
	ptest -> path = NULL;
	return -1;
    }


    manager_init(ptest -> pmanager);
    
    if(ptest -> n_launchers == 0)
	return 0;
    
    
    if((ptest -> launchers = malloc(sizeof(launcher) * ptest -> n_launchers)) == NULL)
    {
	free(ptest -> path);
	manager_delete(ptest -> pmanager);
	free(ptest -> pmanager);
	ptest -> path = NULL;
	ptest -> pmanager = NULL;
	return -1;    
    }


    for(i=0; i<ptest -> n_launchers; i++)
	if(launcher_init(&ptest -> launchers[i], ptest -> path) < 0)
	{	
	    free(ptest -> path);
	    manager_delete(ptest -> pmanager);
	    free(ptest -> pmanager);

	    for(j=i-1; j>=0; j--)
		launcher_delete(&ptest -> launchers[j]);

	    free(ptest -> launchers);

	    ptest -> path = NULL;
	    ptest -> pmanager = NULL;
	    ptest -> launchers = NULL;

	    return -1;
	}

    if((ptest -> times = malloc(sizeof(unsigned int) * ptest -> n_launchers)) == NULL)
    {
	free(ptest -> path);
	manager_delete(ptest -> pmanager);
	free(ptest -> pmanager);
	
	for(i=0; i<ptest -> n_launchers; i++)
	    launcher_delete(&ptest -> launchers[i]);/*??? launcher_delete(&ptest -> launchers[j])*/

	free(ptest -> launchers);

	ptest -> path = NULL;
	ptest -> pmanager = NULL;
	ptest -> launchers = NULL;
	return -1;
    }

    tester_pointer = ptest;

    signal(SIGALRM, alarm_handler);

    return 0;
}


void tester_delete(tester *ptest)
{
    unsigned int i;
    extern tester *tester_pointer;

    if(ptest -> path)
    {
	free(ptest -> path);
	ptest -> path = NULL;
    }

    if(ptest -> pmanager)
    {
	manager_delete(ptest -> pmanager);
	free(ptest -> pmanager);
	ptest -> pmanager = NULL;
    }

    if(ptest -> launchers)
    {
	for(i=0; i<ptest -> n_launchers; i++)
	    launcher_delete(&ptest -> launchers[i]);
	free(ptest -> launchers);
	ptest -> launchers = NULL;
    }

    if(ptest -> times)
    {
	free(ptest -> times);
	ptest -> times = NULL;
    }
    if(tester_pointer == ptest)
	tester_pointer = NULL;
}



int tester_start(tester *ptest)
{
    launcher *plaunch;
    unsigned int n;

    if(ptest -> works != 0)
	return -2;

    alarm(ptest -> time);


    for(;;)
    {
	n = waiting_launcher(ptest);
	plaunch = &(ptest -> launchers[n]);
	
	launcher_setting_from_manager(plaunch, ptest -> pmanager);

	switch(launcher_launch(plaunch, ptest -> act))
	{
	    case -1:
		return -1;
	    case -2:
		continue;
	};
	
	ptest -> times[n] = ptest -> time;
	manager_next_state_bust(ptest -> pmanager);
    }

    return 0;
}


unsigned int waiting_launcher(tester *ptest)
{
    int i;
    for(i=0; ptest -> launchers[i].works != 0;i=(i+1) % ptest -> n_launchers)
	process_launcher(&ptest -> launchers[i], ptest -> pfile);

    return i;
}

void process_launcher(launcher *plaunch, FILE *fp)
{
    int exit_status;

    if(plaunch -> works == 0)
	return;

    if(waitpid(plaunch -> process, &exit_status, WNOHANG) == 0)
	return;

    plaunch -> works = 0;
    plaunch -> process = -1;

/*proccess down*/
    if(exit_status != 0)
	fprintf(fp, "Discover fall program\n");

}

void alarm_handler(int s)
{
    extern tester *tester_pointer;
    unsigned int i;

    /**/
    s = s;
    if(tester_pointer == NULL || tester_pointer -> works == 0 || tester_pointer -> act == user_request)
	return;

    for(i=0; i<tester_pointer -> n_launchers; i++)
    {
	if(tester_pointer -> launchers[i].works)
	    tester_pointer -> times[i]--;
	if(tester_pointer -> times[i] == 0)
	    launcher_stop(&(tester_pointer -> launchers[i]));
    }
}


int tester_init_from_xml(tester *ptest, const char *xml_file_name)
{
    xmlDocPtr doc;
    xmlNodePtr node, node_functions = NULL, node_general = NULL;
    char f_continue;
    char f_test_not_init = 1;
    int ret_val;

    if((doc = xmlParseFile(xml_file_name)) == NULL)
        return -1;

    node = xmlDocGetRootElement(doc);

    if(node == NULL)
    {
        xmlFreeDoc(doc);
        return -1;
    }

    if(xmlStrcmp(node -> name, (const xmlChar *) "settings"))
    {
        xmlFreeDoc(doc);
        tester_init(ptest, "", DEFAULT_NUMBER_OF_PROCESS, DEFAULT_TIME, stdout);
        return -3;
    }


    node = node -> xmlChildrenNode;

    for(f_continue = 0; node; node = node -> next, f_continue = 0)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "tester_settings"))
	{
	    ret_val = init_tester_settings(ptest, node -> xmlChildrenNode, doc);

	    f_test_not_init = 0;
	    if(ret_val != 0)
	    {
		xmlFree(doc);
		return ret_val;		
	    }
	    f_continue = 1;
	}

	if(!xmlStrcmp(node -> name, (const xmlChar *) "functions"))
	    node_functions = node -> xmlChildrenNode;

	if(!xmlStrcmp(node -> name, (const xmlChar *) "general_settings"))
	    node_general = node -> xmlChildrenNode;
    }

    if(node_functions == NULL)
    {
	xmlFree(doc);
	return -1;
    }

    if(f_test_not_init)
        tester_init(ptest, "", DEFAULT_NUMBER_OF_PROCESS, DEFAULT_TIME, stdout);

    
    if((ret_val = init_manager_settings(ptest -> pmanager, node_functions, doc)) != 0)
    {
	xmlFreeDoc(doc);
	return ret_val;
    }

    if((ret_val = init_general_settings(ptest, node_general, doc)) != 0)
    {
	xmlFreeDoc(doc);
	return ret_val;    
    }

    xmlFreeDoc(doc);

    return 0;
}



int init_tester_settings(tester *ptest, xmlNodePtr node, xmlDocPtr doc)
{
    xmlChar *key;
    FILE *out_file = stdout;
    char *path;
    char f_continue;
    unsigned int n_process = DEFAULT_NUMBER_OF_PROCESS, time = DEFAULT_TIME;


    if((path = malloc(sizeof(char))) == NULL)
	return -1;

    path[0] = '\0';

    for(f_continue = 0; node != NULL; node = node -> next, f_continue = 0)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "number_of_process"))
	{
            key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	    n_process = atoi((const char *)key);
    	    xmlFree(key);
	    f_continue = 1;
	}

	if(!f_continue && !xmlStrcmp(node -> name, (const xmlChar *) "program_path"))
	{
	    key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);

	    free(path);
	    if((path = malloc(sizeof(char) * (strlen((const char *) key) + 1))) == NULL)
	    {
		xmlFree(key);
		xmlFreeDoc(doc);
		return -1;
	    }

	    strcpy(path, (const char *) key);
	    xmlFree(key);
	    f_continue = 1;
	}

	if(!f_continue && !xmlStrcmp(node -> name, (const xmlChar *) "max_time_testing"))
	{
	    key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	    time = atoi((const char *) key);
	    xmlFree(key);
	    f_continue = 1;
	}
	
	if(!f_continue && !xmlStrcmp(node -> name, (const xmlChar *) "out_file"))
	{
	    key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);
	    if((out_file = fopen((const char *) key, "w")) == NULL)
	    {
		xmlFree(key);
		xmlFreeDoc(doc);
		free(path);
		return -4;
	    }
	    xmlFree(key);
	    f_continue = 1;
	}
    }

    tester_init(ptest, path, n_process, time, out_file);

    return 0;
}

int init_manager_settings(manager *pman, xmlNodePtr node, xmlDocPtr doc)
{
    xmlChar *key;
    xmlNodePtr node_cur;
    int ret_val;

    for(;node != NULL; node = node -> next)
    {
	if(!xmlStrcmp(node -> name, (const xmlChar *) "function"))
	{
	    for(node_cur = node -> xmlChildrenNode;  node_cur != NULL && xmlStrcmp(node_cur -> name, (const xmlChar *) "name") ; node_cur = node_cur -> next);
	    if(node == NULL)
		return -3;

	    key = xmlNodeListGetString(doc, node_cur -> xmlChildrenNode, 1);    

	    ret_val = manager_add_str(pman, (const char *) key, "0");
	    xmlFree(key);    
	    if(ret_val != 0)
		return ret_val;
	}
    }

    return 0;
}

int init_general_settings(tester *ptest, xmlNodePtr node, xmlDocPtr doc)
{
    xmlChar *key;
    char f;

    for(f = 0;node != NULL && f == 0; node = node -> next)
	if(!xmlStrcmp(node -> name, (const xmlChar *) "behavior"))
	{
	    key = xmlNodeListGetString(doc, node -> xmlChildrenNode, 1);    

	    if(!xmlStrcmp(key, (const xmlChar *) "complete_rejection"))
		ptest -> act = complete_rejection;
	    else if(!xmlStrcmp(key, (const xmlChar *)"rejection" ))
		ptest -> act = rejection;
	    else if(!xmlStrcmp(key, (const xmlChar *) "handling"))
		ptest -> act = handling;
	    else if(!xmlStrcmp(key, (const xmlChar *) "random_behavior"))
		ptest -> act = random_behavior;
	    else if(!xmlStrcmp(key, (const xmlChar *) "user_request"))
		ptest -> act =  user_request;
	    f = 1;
	}

    return 0;
}
