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
	if(strcmp(command,"exit")==0 || strlen(command)==0){  			
		write(STDOUT_FILENO,"bye bye...\n",sizeof("bye bye...\n"));
		return 1;
	}
	return 0;
}

void code_signal(int status){  											//Display the signal of the terminated child
	char signal[100];
	
	if (WIFEXITED(status)){												//The child exited correctly
		sprintf(signal,"[exit:%d]",WEXITSTATUS(status));
	}
	else{ 
		sprintf(signal,"[sign:%d]",WEXITSTATUS(status));
	}
	write(STDOUT_FILENO,signal,strlen(signal));
}


int main(void){
	char command[100];
	int taille;
	int ret;
	int status;
	
	write(STDOUT_FILENO,"Welcome to ENSEA Tiny Shell.\n",29);
	write(STDOUT_FILENO,"To quit, tap 'exit'.\n",21);
	write(STDOUT_FILENO,"enseash %",sizeof("enseash %"));
	
	while (1){
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
		if (ret==0){													//Child process
			
			execlp(command,command,(char *) NULL);
			write(STDOUT_FILENO,"command not found\n",sizeof("command not found\n"));
			kill(getpid(),SIGINT);
		}
		else{															//Parent process
			wait(&status);
			command[0]='\0';
			write(STDOUT_FILENO,"enseash %",sizeof("enseash %"));
			code_signal(status);
				
		}
	}
}


