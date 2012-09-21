#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <unistd.h>

#include "catch_lib.h"

int main(int argc, char **argv)
{
    catch_start(--argc, ++argv);
    for(;;)
    {
	int fd = open("test_file", O_RDONLY | O_CREAT, 511);
        if(fd == -1)
    	    fprintf(stdout, "0\n");
	else
	{
	    fprintf(stdout, "1\n");
	    close(fd);
	}
    	getchar();
    }

    return 0;
}
