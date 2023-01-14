
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char to_string(char command[],int taille){
	if (taille != 0){
		command[taille-1]='\0'; 										// replace '\n' (key ENTER) with a '\0' (EOF)
	}
	return *command;
}

int main(void){
	char command[100];
	int taille;
	int ret;
	int status;
	
	write(STDOUT_FILENO,"Welcome to ENSEA Tiny Shell.\n",29);
	write(STDOUT_FILENO,"To quit, tap 'exit'.\n",21);
	
	while (1){

		write(STDOUT_FILENO,"enseash %",sizeof("enseash %"));

		taille=read(STDIN_FILENO,command,sizeof(command));
		*command=to_string(command,taille);								// replace '\n' (key ENTER) with a '\0' (EOF)

		ret=fork();														//Creation of a child process
		if (ret==-1){													//Case when a child could not be created
			perror("Fork impossible");
			return EXIT_FAILURE;
		}													
		else if (ret==0){ 													// Child process
			execlp(command,command,(char *) NULL);
			write(STDOUT_FILENO,"command not found\n",sizeof("command not found\n"));  //line executed when the command doesn't exist
			kill(getpid(),SIGINT);										//kill the child process
		}
		else{ 															// Parent process
			wait(&status);												// Wait the termination of the child												
			command[0]='\0';											//reset the command variable
		}
	}
}


