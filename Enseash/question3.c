#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char to_string(char command[],int taille){
	if (taille != 0){
		command[taille-1]='\0'; 										
	}
	return *command;
}

int byebye(char command[]){
	if(strcmp(command,"exit")==0 || strlen(command)==0){ 				// When pressed "exit" or <ctrl>+d (which is an EOF so of size 0)
		write(STDOUT_FILENO,"bye bye...\n",sizeof("bye bye...\n"));
		return 1;
	}
	return 0;
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
		
		if (byebye(command)==1){
			return EXIT_SUCCESS;
		}

		*command=to_string(command,taille);	
		
		ret=fork();
		if (ret==-1){													//Case when a child could not be created
			perror("Fork impossible");
			return EXIT_FAILURE;
		}	
		else if (ret==0){  												// Child process
			execlp(command,command,(char *) NULL);
			write(STDOUT_FILENO,"command not found\n",sizeof("command not found\n"));
			kill(getpid(),SIGINT);
		}
		else{  															// Parent process
			wait(&status);
			command[0]='\0';										
		}
	}
}


