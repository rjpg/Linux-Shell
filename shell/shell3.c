#include "shell1.h"


void error (char* erro)   //função que processa os erros
{
   perror(erro);
   exit(-1);
}

void print_cmd_chain(struct command *cmd)  // imprime lista
{
   int i;
 
   for (; cmd != NULL; cmd = cmd->next) {
      printf("Comando: %s\n", cmd->cmd);
      for (i = 0; cmd->argv[i]; i++) {
         printf(" argv[%d]: %s\n", i, cmd->argv[i]);
      }
      if (cmd->input) {
         printf(" input: %s\n", cmd->input);
      }
      if (cmd->output) {
         printf(" input: %s\n", cmd->output);
      }
   }
}

struct command *parse(char *linha)
{
  char *s;
  int i=1;
  lp_command cmd;
  cmd=(lp_command) malloc(sizeof(s_command));
  cmd->input=cmd->output=NULL;
  cmd->next=NULL;
  cmd->cmd=cmd->argv[0]=strtok(linha," ");
  while ((s=strtok(NULL," "))!=NULL)
    {
      cmd->argv[i++]=s;
    }
  return cmd;
}

void correr_cmd(lp_command cmd)
{
  int i=0;
  if((i=fork())==0) //está no filho
    {
      execvp(cmd->cmd,cmd->argv);
      error("execvp");
    }
  else if (i==-1) error("não entrou no filho");
  else   
  wait(NULL);
}


main ()
{
   char *linha;
   lp_command lista;

   printf ("\n(c) X-prog 2001 (Trabalho de S.O.) Shell de comandos\n");
   for (;;) {
      /* Ignora linhas vazias */
      while (strlen(linha = readline("$ ")) == 0) {
         free(linha);         /* Obrigatótio */
      }
      add_history(linha);

      if(strcmp(linha,"exit")==0)
	exit (0);
	
      lista = parse (linha);
      correr_cmd(lista);

      print_cmd_chain(lista);
      //   printf("%s\n", linha);
      free(linha);            /* Obrigatório */
   }
}
