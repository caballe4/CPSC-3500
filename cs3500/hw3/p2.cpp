//Jack Gularte Anthony Caballero

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

struct process{
	int pid;
	int burst_time;
	int arrival_time;
};

void FCFS(int numProc)
{
	int total_time = 0;
	
	for(int i = 0; i < numProc; i+=){
		while(process.burst_time != 0){
			process.burst_time--;
			total_time++;
		}
	}
	
}


void roundRobin(int numProc, int argc)
{
	int total_time = 0;
	
	for(int i = 0; i < numProc; i+=){
		while(process.burst_time != 0 && ){
			process.burst_time--;
			total_time++;
		}
	}
}