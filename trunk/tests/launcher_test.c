#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>
#include <pthread.h>

#include  "../launcher.h"
#include "../resource.h"
#include "../bigbool.h"

launcher my_launch;

void filling(launcher *plaunch, int n_repit)
{
    char name[1024];
    bigbool bbool;
    int i, j, n;
    for(i=0; i<n_repit; i++)
    {
	n = rand() % 999 + 24;
	for(j=0; j<n; j++)
	    name[j] = rand() % 24 + 'A';
	name[j] = 0;

	n = rand() % 1000;

	bigbool_init(&bbool);	
	bigbool_create(&bbool, n);
    
	for(j=0; j<n; j++)
	    bigbool_set(&bbool, rand() % n, 1);

        launcher_add_resource(plaunch, name, &bbool);
	bigbool_delete(&bbool);
    }
}





void test1(int n_repit)
{
    launcher launch;
    int i;
    char *name = "name";
    for(i=0; i<n_repit; i++)
    {
	if(i % 100 == 0)
	    fprintf(stderr, "%d\n", i);
	launcher_init(&launch, name);
	filling(&launch, rand() % 100 + 10);
	launcher_delete(&launch);
    }
}



void test2(void)
{
    launcher launch;
    char tmp_str[1024];
    char tmp_bool[1024];
    unsigned int i, n, j;
    bigbool bbool;

    fprintf(stdout, "Path: ");
    fgets(tmp_str, 1024, stdin);
    tmp_str[strlen(tmp_str) - 1] = 0;
    
    launcher_init(&launch, tmp_str);
    bigbool_init(&bbool);


    fprintf(stdout, "Size: ");
    fscanf(stdin, "%o", &n);
    getchar();

    for(i=0; i<n; i++)
    {
	fgets(tmp_str, 1024, stdin);
	fgets(tmp_bool, 1024, stdin);
	tmp_bool[strlen(tmp_bool) - 1] = 0;
	tmp_str[strlen(tmp_str) - 1] = 0;
	bigbool_create_str(&bbool, tmp_bool);
	launcher_add_resource(&launch, tmp_str, &bbool);
	bigbool_delete(&bbool);
    }
    
    for(;;)
    {
	for(i=0; i<launch.size; i++)
	{
	    fprintf(stdout, "%s: ", launch.res_names[i]);
	
	    for(j=0; j<launch.res_bools[i] -> size_bit; j++)
		fprintf(stdout, "%d", bigbool_get(launch.res_bools[i], j));
	    fprintf(stdout, "\n");
	}

	fgets(tmp_str, 1024, stdin);
	tmp_str[strlen(tmp_str) - 1] = 0;

	if(strcmp(tmp_str, "end") == 0)
	    break;

	launcher_del_resource(&launch, tmp_str);
    }

    launcher_delete(&launch);

}


void test3()
{
    launcher launch;
    char tmp_str[1024];
    char tmp_bool[1024];
    int i, n, status;
    bigbool bbool;

    fprintf(stdout, "Path: ");
    fgets(tmp_str, 1024, stdin);
    tmp_str[strlen(tmp_str) - 1] = 0;
    
    launcher_init(&launch, tmp_str);
    bigbool_init(&bbool);


    fprintf(stdout, "Size: ");
    fscanf(stdin, "%d", &n);
    getchar();

    for(i=0; i<n; i++)
    {
	fgets(tmp_str, 1024, stdin);
	fgets(tmp_bool, 1024, stdin);
	tmp_bool[strlen(tmp_bool) - 1] = 0;
	tmp_str[strlen(tmp_str) - 1] = 0;
	bigbool_create_str(&bbool, tmp_bool);
	launcher_add_resource(&launch, tmp_str, &bbool);
	bigbool_delete(&bbool);
    }
    

    launcher_launch(&launch, user_request);

    waitpid(launch.process, &status, 0);
    fprintf(stderr, "launcher end. Code: %d", status);

    launcher_delete(&launch);
}


void *thread_func(void *args)
{
    /**/
    if(args)
    {
    }

    action act = rejection;

    fprintf(stderr, "launch\n");  
    launcher_launch(&my_launch, act);
    fprintf(stderr, "end launch\n");
    return NULL;
}



void test4(void)
{
    pthread_t thread;
    launcher_init(&my_launch, "inf_loop");
    pthread_create(&thread, NULL, &thread_func, NULL);    
    sleep(5);
    fprintf(stderr, "kill, %d\n", my_launch.process);
    launcher_stop(&my_launch);
}



void test5(void)
{
    unsigned int n, i, j;
    char s1[1024], s2[1024];
    manager man;
    launcher launch;
    resource res;
    bigbool bbool;
    manager_init(&man);
    launcher_init(&launch, "");

    fputs("manager inithialization\nsize=", stdout);
    fscanf(stdin, "%o", &n);
    fgetc(stdin);

    for(i=0; i<n; i++)
    {
	fgets(s1, 1024, stdin);
	s1[strlen(s1)-1] = '\0';
	
	fgets(s2, 1024, stdin);
	s2[strlen(s2)-1] = '\0';

	resource_init(&res, s1, s2);	
	
	manager_add(&man, &res);
	resource_delete(&res);
    }


    fputs("launcher inithialization\nsize=", stdout);
    fscanf(stdin, "%o", &n);
    fgetc(stdin);

    for(i=0; i<n; i++)
    {
	bigbool_init(&bbool);
	fgets(s1, 1024, stdin);
	s1[strlen(s1)-1] = '\0';

	fgets(s2, 1024, stdin);
	s2[strlen(s2)-1] = '\0';

	bigbool_create_str(&bbool, s2);
	launcher_add_resource(&launch, s1, &bbool);
	bigbool_delete(&bbool);
    }

    fputs("start settings\n", stdout);

    launcher_setting_from_manager(&launch, &man);

    fputs("end settings\n", stdout);

    for(i=0; i<launch.size; i++)
    {
	fprintf(stdout, "%s ", launch.res_names[i]);
	for(j=0; j<launch.res_bools[i] -> size_bit; j++)
	    fprintf(stdout, "%d", bigbool_get(launch.res_bools[i], j));
	fprintf(stdout, "\n");
    }
}


int main(void)
{
    srand(time(NULL));
    test3();
    return 0;
}
