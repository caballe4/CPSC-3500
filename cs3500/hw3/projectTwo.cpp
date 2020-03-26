/*
Author: Jack Gularte and Anthony Caballero
Filename: projectTwo.cpp
For: CPSC 3500
Date: January 29 2019
Compiles With: g++ projectTwo.cpp
*/


#include "projectTwo.h"
using namespace std;

// main
int main(int argc, char* argv[]) { 
	
	// setup
	int num_procs = 0;
	int time = 0;
	
	//pointers
	char *filename, *sched_type;
	process_struct* processes[MAX_PROCS];
	
	// primitives
	int t_quantum;
	bool funct_success;
	
	
	// check for errors in the command line, if true exit with error
	if (parse_error(argc, argv)) {
		return 1;
	}
	
	// assign variables the commandline arguments
	filename = parse_filename(argv);
	sched_type = parse_sched(argv);
	
	// test for if time quantum was passed in
	if (argc == IS_QUANTUM)
		t_quantum = parse_quantum(argv);
	
	// parse the input file and load the data into memory
	// if input fails it will return false
	num_procs = input_file(filename, processes);

	// bool change = sort_by_arrival(processes, 0, 5);
	sort_by_arrival(processes, LOW_START, num_procs - 1);
	
	if(strcmp(argv[2],"FCFS") == 0)
		FCFS(processes, num_procs);
	time = 0;
	if(strcmp(argv[2],"RR")==0)
		Round_Robin(processes,t_quantum, num_procs);
	if(strcmp(argv[2],"SRTF") == 0)
		shortest(processes,num_procs);


	
    return 0;
}

void FCFS(process_struct* processes[], int num_procs)
{
	int total_time = 0;
	int count = 0;	
	int total;

	//FCFS
	for(int i = 0; i < num_procs ; i++){
		while(processes[i]->burst_time != 0){
			processes[i]->burst_time--;
			total_time++;
			count+=processes[i]->burst_time;
			cout << "Process " << processes[i]->pid << " is running" << endl;
		}
		cout << endl << "Process " << processes[i]->pid << " is finished." << endl << endl << endl;
		total_time = 0;
	}
	total = count + processes[0]->burst_time;
	float wait_time = count/num_procs;
	float tat = total/num_procs;
	cout << "Average Wait Time: " << wait_time << endl;
	cout << "Average Turnaround Time: " << tat << endl; 
}

void Round_Robin(process_struct* processes[], int q, int n)
{
	int rt[n];
	bool flag[n];
	float wait_time = 0;
	float tat = 0;

	int total_time = 0;
	for(int i = 0; i < n; i++){
		rt[i] = processes[i]->burst_time;
		flag[i] = false;
	}
	int j = 0;
	while(j < n){
		for(int i = 0; i < n; i++){
			if(rt[i] > 0 && rt[i] <= q){
				total_time += rt[i];
				rt[i] = 0;
				flag[i] = true;
				j++;
				wait_time += total_time - processes[i]->arrival_time - processes[i]->burst_time;
				tat += total_time - processes[i]->arrival_time;
				cout << "Process " << processes[i]->pid << " is finished" << endl;
			}else if(rt[i] > 0){
				cout << "Process " << processes[i]->pid << " is running" << endl;
				rt[i] -= q;
				total_time += q;
			}
		}
	}
	cout << "Average Wait Time: " << wait_time/n << endl;
	cout << "Average Turnaround Time: " << tat/n << endl;
}

void shortest(process_struct* processes[], int n) {
	
	double avg = 0;
	double tat = 0;;
	int rt[n];
	int min = 0;
	int end = 0;
	int count = 0;
	int total_time = 0;

	for(int i = 0; i < n; i++){
		rt[i] = processes[i]->burst_time;
	}
	//for(total_time= 0; count != n; total_time++){
	//	min = n-1;
		//sleep(1);
		//cout << "test" << endl;
	while(count != n){
		//int i = 0;
		for(int j = 0; j < n; j++){
			if(processes[j]->arrival_time <= total_time &&
				processes[j]->burst_time < processes[min]->burst_time &&
				processes[j]->burst_time > 0){
				min = j;
			}
			//cout << "test2" << endl;
			processes[min]->burst_time--;
		cout << "Process " << processes[min]->pid << " is running" << endl;
		}
		
		//if(min < n)
			//min++;
		if(processes[min]->burst_time == 0){
			count++;
			end = total_time + 1;
			cout << "Process " << processes[min]->pid << " is finished" << endl;
			avg = avg + end - processes[min]->arrival_time
				-rt[min];
			tat = tat + end - processes[min]->arrival_time;
		}
	}
	cout << "Average Wait Time: " << avg/n << endl;
	cout << "Average Turnaround Time: " << tat/n << endl;

}

// Error checking function to make sure correct number of 
// command line arguments are given before attempting to parse
bool parse_error(int argc, char** argv) {
	
	// if none, not enough, or too many
	if (argv == NULL || argc < 2 || argc > 4) {
		cout << "[PARSE_ERROR] Commandline has an incorrect number of arguments. " << endl;
		cout << "Must be given 2 or 3 extra arguments. " << endl;
		
		return true;
	}
	
	// if STRF, check that a quantum is given
	if (strcmp(argv[2], "STRF") == 0) {
		if (argc != 4) {
			cout << "[PARSE_ERROR] Schedule Type STRF has been requested but " << endl;
			cout << "no time quantum is given by user" << endl;
			
			return true;
		}
	}
	
	return false;
}

// parse the commandline for the filename and return;
char* parse_filename(char** argv) {
	return argv[FILE_INDEX];
}

// parse the commandline for the schedule type
char* parse_sched(char** argv) {
	return argv[SCHED_INDEX];
}

// parse the commandline for the quantum number
int parse_quantum(char** argv) {
	return atoi(argv[QUANTUM_INDEX]);
}

// Read the given input file into a useable format
int input_file(char* filename, process_struct** processes) {
	
	// set up
	ifstream input("input1.txt");
	string line;
	string temp_string;
	int array_index = 0;
	int variable_index = 0;
	int store_row[READ_ARGS];
	
	// iterate through line by line
	while (getline(input, line)) {	
		stringstream line_stream(line);
		while(getline(line_stream, temp_string, ',')) {	
			store_row[variable_index] = atoi(temp_string.c_str());
			variable_index++;
		}
		
		// initalize a new struct
		// find a better way to do this. this is r/badcode material
		processes[array_index] = new process_struct(0, 0, 0);
		processes[array_index]->pid = store_row[0];
		processes[array_index]->arrival_time = store_row[1];
		processes[array_index]->burst_time = store_row[2];
		
		// increment
		variable_index = 0;
		array_index++;
	}

	return array_index;
}

// partition used for quicksort
int partition (process_struct** processes, int low, int high) 
{ 	
	// temp struct
	process_struct* temp = new process_struct(0, 0, 0);
	
    int pivot = processes[high]->arrival_time;    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (processes[j]->arrival_time <= pivot) 
        { 
            i++;    // increment index of smaller element 
			temp = processes[i];
			processes[i] = processes[j];
			processes[j] = temp;

        } 
    } 
	
	// perform the last swap
	temp = processes[i + 1];
	processes[i + 1] = processes[high];
	processes[high] = temp;
	
    return (i + 1); 
} 

// sort the array with quicksort by arrival time
void sort_by_arrival(process_struct** processes, int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(processes, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        sort_by_arrival(processes, low, pi - 1); 
        sort_by_arrival(processes, pi + 1, high); 
    } 
} 