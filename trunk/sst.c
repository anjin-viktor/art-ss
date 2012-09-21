#include <stdio.h>
#include <getopt.h>

#include "tester.h"
#include "manager.h"
#include "create_lib.h"


tester test;

FILE *pf_out = NULL;

const char *help = 
"Usage ./s_test [OPTION]...\n"
"-h --help			печать справки и выход\n"
"-p --program <path>		путь к тестируемой программе\n"
"-t --max_time <time>		максимальное время тестирования одного процесса\n"
"-o --out_file <path>		файл для вывода информации о ошибках\n"
"-l --create_lib			построение библиотеки для перехвата\n"
"-b --behavior <behavior>	указание поведения тестируемой программе (complete_rejection, rejection, handling, random_behavior, user_request)\n"
"-c --config <path>		путь к файлу конфигурации (обязательный пареметр)\n\n";


/*
    --config
    --number_of_process
    --program
    --max_time
    --out
    --create_lib
    --behavior
*/

void init(int argc, char **argv)
{
    const struct option long_opts[] = 
    {
	{ "behavior", required_argument, NULL, 'b' },
	{ "create_lib", no_argument, NULL, 'l' },
	{ "help", no_argument, NULL, 'h' },
        { "config", required_argument, NULL, 'c' },
        { "program", required_argument, NULL, 'p' },
        { "max_time", required_argument, NULL, 't' },
        { "out", required_argument, NULL, 'o' }
    };

    const char *short_opts = "b:lc:p:t:o:h";
    int option;
    char *pc = NULL, *pprog = NULL, *pt = NULL, *po = NULL, *pb = NULL;
    char f = 0;

    for(option = 0; (option = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1;)
	switch(option)
	{
	    case 'c':
		pc = optarg;		
		break;

	    case 'p':
		pprog = optarg;
		break;

	    case 't':
		pt = optarg;
		break;

	    case 'o':
		po = optarg;
		break;
	    
	    case 'h':
		fputs(help, stdout);
		exit(0);

	    case 'l':
		f = 1;	
		break;
	
	    case 'b':
		pb = optarg;
		break;

	    case '?':
		break;
	};



    if(pc == NULL)
    {
	fputs(help, stderr);
	exit(1);
    }
    
    if(f)
    {
	if(create_lib(pc) < 0)
	{
	    fprintf(stderr, "Ошибка при генерации библиотеки по файлу %s\n", pc);
	    tester_delete(&test);
	    exit(2);
	}
	exit(0);    
    }
    
    

    if(tester_init_from_xml(&test, pc) < 0)
    {
	fprintf(stderr, "Ошибка при инициализации из xml-файла %s:\n", pc);
	tester_delete(&test);
	exit(2);
    }


    if(pt != NULL)
	test.time = atoi(pt);

    if(po != NULL)
    {
	if(test.pfile)
	    fclose(test.pfile);

	if((test.pfile = fopen(po, "w")) == NULL)
	{
	    fprintf(stderr, "Ошибка при открытии файла %s\n", po);
	    tester_delete(&test);
	    exit(3);
	}
    }


    if(pprog != NULL)
    {
	int len = strlen(pprog) + 1;
	unsigned int i;

	for(i=0; i<test.n_launchers; i++)
	{
	    free(test.launchers[i].path);

	    if((test.launchers[i].path = malloc(sizeof(char) * len)) == NULL)
	    {
		fputs("Системная ошибка при работе программы\n", stderr);
		tester_delete(&test);
		exit(4);
	    }
	    strcpy(test.launchers[i].path, pprog);
	}

	free(test.path);
	if((test.path = malloc(sizeof(char) * len)) == NULL)
	{
	    fputs("Системная ошибка при работе программы\n", stderr);
	    tester_delete(&test);
	    exit(4);
	}

	strcpy(test.path, pprog);
    }

    if(pb != NULL)
    {
        if(!strcmp(pb, "complete_rejection"))
            test.act = complete_rejection;
        else if(!strcmp(pb, "rejection" ))
            test.act = rejection;
        else if(!strcmp(pb, "handling"))
            test.act = handling;
        else if(!strcmp(pb, "random_behavior"))
            test.act = random_behavior;
        else if(!strcmp(pb, "user_request"))
            test.act =  user_request;
    }
}




int main(int argc, char **argv)
{
    init(argc, argv);

    if(tester_start(&test) < 0)
    {
	fputs("Ошибка при запуске теста\n", stderr);
	tester_delete(&test);
	return 5;
    }
    
    fclose(test.pfile);
    tester_delete(&test);
    return 0;
}
