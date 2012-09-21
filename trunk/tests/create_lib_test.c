#include <stdio.h>

#include "create_lib.h"

int main(int argc, char **argv)
{
    extern char create_lib_error [];
    if(argc < 2)
    {
	fprintf(stderr, " argv[1] = xml_file_name");
	return 1;
    }
    create_lib(argv[1]);
    fprintf(stderr, "%s\n", create_lib_error);

    return 0;
}
