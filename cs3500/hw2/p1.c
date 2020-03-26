/*Anthony Caballero
p1.c
CPSC 3500
23 January 2019

Purpose: Creating a simple shell that can handle multiple commands 
separated with pipes*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFF_LEN 1024
#define MAX_COMMANDS 10
#define ROWS 50
#define COLS 100

struct command{
	const char** argv;
};

void execute (int x, struct command* cmd){
	int i;
	pid_t p1;
	int fd[2];
	int input = 0;

	//sending correct amount of arguments to pipe_execute
	//for loop will only run x times with x being
	//number of pipes counted in parse()
	for (i = 0; i < x; i++) {
		pipe (fd);
		
		p1 = pipe_execute(input, fd[1], cmd + i);
		printf("Process %d", getpid());
		//closing unnecessary file desprictors
		close (fd[1]);
		input = fd [0];
	}
	if (input != 0){
		dup2 (input, 0);
		printf("Process %d", getpid());
	} 
	//runs command located in "i"th element of of struct array.
	//typecasting second argument in execvp for unity
	//error checks for failure of execvp
     if(!(execvp(cmd[i].argv[0], (char* const*) cmd[i].argv)))
             perror("Failed to execute.\n");
} 

pid_t pipe_execute (int input, int output, struct command* cmd){
	pid_t p1;
    p1 = fork();
	int stat;
	
	//error check for success of fork
	if(p1 < 0)
		perror("Fork failed \n");
	//child
	if(p1 == 0){
		//duplicating necessary code while closing unused descriptors
		if (input != 0) {
			dup2 (input, 0);
			close (input);
		} 
		if (output != 1) {
			dup2 (output, 1);
			close (output);
		}
		//printing process id's 
		printf("Process %d", getpid());
		//executing command and error checking
        if(!(execvp( cmd->argv[0], (char* const *)cmd->argv)))
			perror("Failed to Execute.\n");
	}
	return p1;

}

void parse()
{
	char line[BUFF_LEN];
	char buff[ROWS][COLS];
	int length;
	int i = 0, row = 0, col = 0, count = 0;
	char* args[ROWS];
	char* commandLine[MAX_COMMANDS][MAX_COMMANDS];
	
	//fails if input isn't received correctly
	if(!fgets(line,BUFF_LEN,stdin)){
		perror("fgets error \n");
		return;
	}
	//determine length of input
	length = strlen(line);
	
	//traversing "line" to filter unwanted characters
	while(i < (length -1)){
		//writes over single quotes and copies letters into buff array until next quote found
		if(line[i] == '\''){
			i++;
			while(line[i] != '\''){
				buff[row][col] = line[i];
				col++;
				i++;
			}
			//adding null to end of argument and incrementing appropriately
			buff[row][col] = '\0';
			col = 0;
			row++;
			i++;
		}
		//similar logic but with double quotes
		else if(line[i] == '\"'){
			i++;
			while(line[i] != '\"'){
				buff[row][col] = line[i];
				i++;
				col++;
			}
			buff[row][col] = '\0';
			row++;
			i++;
			col = 0;
		}
		//search for pipe. Signifies new command
		//'|' will be only element in this row
		else if(line[i] == '|'){
			count++;
			buff[row][col] = '|';
			buff[row][col+1] = '\0';
			col = 0;
			row++;
			i++;
		}
		//parse command for sequence of acceptable characters
		else if(line[i] != ' '){
			while(line[i] != ' ' && i < (length -1) && line[i] != '|'){
				buff[row][col] = line[i];
				i++;
				col++;
			}
			buff[row][col] = '\0';
			row++;
			col = 0;
		}else
			i++;
	}
	//creating array of arguments using pointer
	int j;
	for(j = 0; j < row; j++)
		args[j] = (char*)buff[j];
	args[row] = (char*)NULL;

	i = 0;
	int numArgs = 0;
	int val = 0;
	for (i = 0; i < row; i++){
		if (buff[i][0] == '|'){
			commandLine[numArgs][val] = NULL;
			numArgs++;
			val = 0;		
		} else {
			commandLine[numArgs][val] = args[i];
			val++;
		}
	}
	commandLine[numArgs][val] = NULL;
	
	//creation of structure array to hold 10 arguments
	//helps send correct number of arguments to execute()
	struct command cmd[] = {commandLine[0], commandLine[1], commandLine[2], commandLine[3],
						commandLine[4], commandLine[5], commandLine[6], commandLine[7],
						commandLine[8], commandLine[9]};

	//sending pipe count and structure array to execute commands
	 execute(count, cmd);
}


int main()
{
		while(1){
		printf("Shell>> ");

		parse();
	}
	return 0;	
}



	
	
	
	
	
	
	