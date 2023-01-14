
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void){
	char command;
	write(STDOUT_FILENO,"Welcome to ENSEA Tiny Shell.\n",29);			
	write(STDOUT_FILENO,"To quit, tap 'exit'.\n",21);
	write(STDOUT_FILENO,"enseash %\n",10);
	exit(EXIT_SUCCESS);
}
