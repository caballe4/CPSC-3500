/*
Author: Jack Gularte and Anthony Caballero
Filename: projectTwo.h
For: CPSC 3500
Date: January 29 2019
*/

#ifndef PROJECT_TWO
#define PROJECT_TWO

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// Sizes
#define MAX_ARGS 3
#define FILE_NAME_SIZE 15
#define MAX_PROCS 50
#define TYPE_SIZE 4
#define STRUCT_ARRAY_SIZE 50
#define READ_ARGS 3

// locations
#define LOW_START 0
#define FILE_INDEX 1
#define SCHED_INDEX 2
#define QUANTUM_INDEX 3

// checks
#define IS_QUANTUM 4

struct process_struct {
	int pid, arrival_time, burst_time;
	
	process_struct(int n_pid, int n_arrival_time, int n_burst_time) {
		pid = n_pid;
		arrival_time = n_arrival_time;
		burst_time = n_burst_time;
	}
	
	// overloaded
	process_struct& operator=(const process_struct &rhs) {
		pid = rhs.pid;
		arrival_time = rhs.arrival_time;
		burst_time = rhs.burst_time;
	}
	
};

int main(int argc, char* argv[]);
bool parse(int argc, char** argv, char** filename, char** sched_type, int &t_quantum);
bool parse_error(int argc, char** argv); 
char* parse_filename(char** argv);
char* parse_sched(char** argv);
int parse_quantum(char** argv);
int   input_file(char* filename, process_struct** processes);
void sort_by_arrival(process_struct** processes, int low, int high);
//void sort_by_remaining(process_sruct** processes, int low, int high);
int partition (process_struct** processes, int low, int high);
void FCFS(process_struct* processes[], int num_procs);
void Round_Robin(process_struct* processes[], int q, int n);
void shortest(process_struct* processes[], int n);

#endif
