#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

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

void code_signal(int status,struct timespec *prev_time,struct timespec *next_time){
	char signal[100];
	int time;
	
	time=(int)(((double) next_time->tv_nsec)-((double) prev_time->tv_nsec))/1000000;  //time of the execution (conversion from ns to ms)
	
	if (WIFEXITED(status)){
		sprintf(signal,"[exit:%d|%d ms]",WEXITSTATUS(status),time);
	}
	else{
		sprintf(signal,"[sign:%d|%d ms]",WEXITSTATUS(status),time);
	}
	write(STDOUT_FILENO,signal,strlen(signal));
}


int main(void){
	char command[100];
	int taille;
	int ret;
	int status;
	struct timespec prev_time;
	struct timespec next_time;	
	
	write(STDOUT_FILENO,"Welcome to ENSEA Tiny Shell.\n",29);
	write(STDOUT_FILENO,"To quit, tap 'exit'.\n",21);
	write(STDOUT_FILENO,"enseash %",sizeof("enseash %"));
	
	while (1){
		taille=read(STDIN_FILENO,command,sizeof(command));
		
		if (clock_gettime(CLOCK_REALTIME,&prev_time)==-1){				//get time of the beginning of the execution
			perror("clock gettime");
			return EXIT_FAILURE;
		}
		
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
			if (clock_gettime(CLOCK_REALTIME,&next_time)==-1){			//get time when the execution is finish (meaning the child is terminated)
				perror("clock gettime");
				return EXIT_FAILURE;
			}
			command[0]='\0';
			write(STDOUT_FILENO,"enseash %",sizeof("enseash %"));
			code_signal(status,&prev_time,&next_time);
				
		}
	}
}


