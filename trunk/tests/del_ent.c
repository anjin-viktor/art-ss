#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char ch;

    if(argc != 2)
    {
	fprintf(stderr, "argv[1] - file_name\n");
	return 1;
    }

    if((fp = fopen(argv[1], "r")) == NULL)
    {
	return -2;
    }

    fputc('\n', stdout);
    
    ch = fgetc(fp);

    while(!feof(fp))
    {
	if(ch == '\n')
	    fputs("\\n", stdout);
	else
	    fputc(ch, stdout);
	ch = fgetc(fp);
    }

    fputc('\n', stdout);

    fclose(fp);
    return 0;
}
