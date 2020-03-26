Anthony Caballero
README hw2
CPSC 3500

The purpose of this project was to create a simple shell in that can run either one or multiple commands.
This is to exercise our knowledge of the kernal comands such as fork(), pipe() and dup2(). The shell can 
close after the command or pipeline of commands has completed. Along with executing the command, the output
will include the process ID numbers as well as exit status of every process that ran. 

In order to compile, use the Makefile given. Run the command "make p1"
This uses a gcc compiler.

To run the project, type "./p1".

Strengths:
	Parse function is able to eliminate all unwanted characters.
	Creates a useable character array as a list of arguments
	Successfully runs single and piped commands
	Error checks with seemingly no memory leaksks. 
	
Weaknesses:
	Began parsing using the strtok() function and ran into problems with piping
	Had to recreate the parse function to iterate one element at a time. 
	Slow in terms of run time as well as takes up much memory.
	Only prints the process ID on certain occassions and not with every execution. 
	When using strtok for parsing, it didn't terminate after one entry, however after
	redoing the parse function, will only run one entry. 
	Parse is lengthy and rigid because of the low level logic. 
	
Team Members:
	Anthony Caballero