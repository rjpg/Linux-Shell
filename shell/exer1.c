#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>   
#include <string.h> //para o strlen  
#include <stdlib.h>

int main (int argc,char *argv[])
{
   printf ("\n(c) X-prog 20001 (Trabalho de S.O.) Shell de comandos\n");
        if (argc!=1)
                {
                printf ("EX:  shell\n\n");
                return 1;
                }

        //printf("Input file: %s",argv[1]);
        //printf("    Output file: %s\n\n",argv[2]);
}
