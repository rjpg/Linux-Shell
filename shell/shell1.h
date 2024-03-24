#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>  // para o execvp() e getcwc()
#include <string.h> //para o strlen  
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>     
#include <signal.h> // alinha 6 (control z...)

#include <readline/readline.h>  //ler linhas 
#include <readline/history.h>

#define MAXARGS 100       //numero max de argumentos 

typedef struct command {
   char *cmd;              /* String apenas com o comando */
   char *argv[MAXARGS+1];  /* Vector de argumentos argv[] */
   char *input;            /* String para redireccionamento de input */
   char *output;           /* String para redireccionamento de output */
   struct command *next;   /* Proximo comando na pipe */
}*lp_command, s_command;

void error (char* erro); //mortra erro e sai 
struct command *parse(char *linha); // cria a lista 
void print_cmd_chain(struct command *cmd); // imprime a lista 
void correr_cmd(lp_command); // corre uma estrututa command

