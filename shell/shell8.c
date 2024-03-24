#include "shell1.h"


void error (char* erro)   //função que processa os erros
{
   perror(erro);
   exit(-1);
}

void print_cmd_chain(struct command *cmd)  // imprime lista
{
   int i;
   printf("\n");
 
   for (; cmd != NULL; cmd = cmd->next) 
   {
     printf("Comando: %s\n", cmd->cmd);
     for (i = 0; cmd->argv[i]; i++) {
         printf(" argv[%d]: %s\n", i, cmd->argv[i]);
     }
     if (cmd->input) {
       printf(" input: %s\n", cmd->input);
     }
      if (cmd->output) {
	printf(" output: %s\n", cmd->output);
      }
   }
   printf("\n");
}

struct command *parse(char *linha)
{
  char *s;
  int i=1;
  lp_command cmd,rcmd;
  cmd=(lp_command) malloc(sizeof(s_command));
  cmd->input=cmd->output=NULL;
  cmd->next=NULL;
  rcmd=cmd;
  cmd->cmd=cmd->argv[0]=strtok(linha," ");
  
  while ((s=strtok(NULL," "))!=NULL)
    {
      if(*s=='|')
	{
	  s++;
	  cmd->next=(lp_command) malloc(sizeof(s_command));
	  cmd=cmd->next;
	  cmd->input=cmd->output=NULL;
	  cmd->next=NULL;
	  cmd->cmd=cmd->argv[0]=strtok(NULL," "); 
	  i=1;
	}
      else 
	{
	  if(*s=='<')    // input
	    {
	      if ((s=strtok(NULL," "))==NULL)
		{
		  fprintf (stderr,"falta ficheiro");
		  break;
		}
	      cmd->input=s;
	      if ((s=strtok(NULL," "))!=NULL)  // a seguir a '<' pode vir '>'
		if(*s=='>')
		  {
		    if ((s=strtok(NULL," "))==NULL)
		      {
			fprintf (stderr,"falta ficheiro");
			break;
		      }
		    cmd->output=s;
		  }
	      
	      break;
	    }
	  
	  if(*s=='>')    // output 
	    {
	      if ((s=strtok(NULL," "))==NULL)
		{
		  fprintf (stderr,"falta ficheiro");
		  break;
		}
	      
	      cmd->output=s;
	      break;
	    }
	  cmd->argv[i++]=s;
	}
    }
  return rcmd;
}

void liberta_lcmd (lp_command cmd)
{
  lp_command aux;
  while (cmd!=NULL)
    {
      aux=cmd->next;
      free(cmd);
      cmd=aux;
    }
}

void correr_cmd(lp_command cmd)
{
  int i=0,x=1;
  int ficheiro;
  if (strcmp(cmd->cmd,"cd")==0)
    {
      if (chdir(cmd->argv[1])==-1)
	printf("\ndirectorio imvalido\n");
      return;
    }
  
  if (strcmp(cmd->cmd,"exit")==0)
    {
      printf ("\n Bye bye\n");
	  exit (0);
    } 
  
  if((i=fork())==0) //está no filho
    { 
      signal (SIGINT,SIG_DFL); // alinha 6
      if (cmd->input!=NULL)
	{
	  if((ficheiro=open(cmd->input,O_RDONLY))==-1)
	    fprintf (stderr,"Não abri o ficheiro...");
	  else
	    {
	      dup2(ficheiro,0);         //dupicar o fich. para o stdin (0)
	      close (ficheiro);
	    } 
	}
      
      if (cmd->output!=NULL)
	{
	  if((ficheiro=open(cmd->output,O_TRUNC | O_WRONLY ))==-1)
	    fprintf (stderr,"Não abri o ficheiro...");
	  else
	    {
	      dup2(ficheiro,1);         //dupicar o fich. para o stdout (1)
	      close (ficheiro);
	    } 
	}
      
      if (strcmp(cmd->cmd,"echo")==0)
	{
	  while (cmd->argv[x]!=NULL)
	    printf("%s ",cmd->argv[x++]);
	  printf("\n");
	}
      else  if (strcmp(cmd->cmd,"pwd")==0)
	{
	  printf("%s\n",get_current_dir_name());
	}
      else 
	{
	  execvp(cmd->cmd,cmd->argv);
	  error("execvp");
	}
      exit (0);  // SAIR DO FILHO DEPOIS DE EXECUTAR O COMANDO !!
    }
  else if (i==-1) error("não entrou no filho");
  else  wait(NULL);
}


main ()
{
   char *linha;
   lp_command lista;
   char prompt[60];

   //signal (SIGTSTP,SIG_IGN); // alinha 6
   //signal (SIGINT,SIG_IGN);

   printf ("\n(c) X-prog 2001 (Trabalho de S.O.) Shell de comandos\n\n");
   for (;;) {
      /* Ignora linhas vazias */
     sprintf(prompt,"X:%s/>",get_current_dir_name());
     while (strlen(linha = readline(prompt)) == 0)
      {
         free(linha);         /* Obrigatótio */
      }
      add_history(linha);

      lista = parse(linha); // parce para a lista de comandos
      
      correr_cmd(lista); // corre lista de commndos
      
      print_cmd_chain(lista); // imprime lista de comandos
      
      liberta_lcmd(lista); // liberta a lista de comandos

   } 
   free(linha);            /* Obrigatório */
}
