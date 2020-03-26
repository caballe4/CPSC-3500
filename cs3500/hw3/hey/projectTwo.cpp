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
	
	//pointers
	char *filename, *sched_type;
	process_struct* processes[50];
	
	// primitives
	int t_quantum = 0;
	bool funct_success;
	
	
	// check for errors in the command line, if true exit with error
	if (parse_error(argc, argv)) {
		return 1;
	}
	
	// assign variables the commandline arguments
	filename = parse_filename(argv);
	sched_type = parse_sched(argv);
	
	// TODO: decide how i want to check for this condition
	if (argc == 4)
		t_quantum = parse_quantum(argv);
	
	// parse the input file and load the data into memory
	bool changeLater = input_file(filename, processes);
	
	int total_time = 0;
	
	int min = process[0].arrival_time;
	
	for(int i = 0; i < 6 ; i++){
		while(process[i]burst_time != 0){
			process[i].burst_time--;
			total_time++;
		}
		cout << "TEMP[" << i << "] PID: " << processes[i].pid << "\t";
		cout << "TEMP[" << i << "] START: " << processes[i].arrival_time << "\t";
		cout << "TEMP[" << i << "] BURST: " << processes[i].burst_time << endl;
		cout << "Total time: " << total_time << endl;

	}
	
	
	/*for (int i = 0; i < 6; i++) {
		
		cout << "TEMP[" << i << "] PID: " << processes[i]->pid << "\t";
		cout << "TEMP[" << i << "] START: " << processes[i]->arrival_time << "\t";
		cout << "TEMP[" << i << "] BURST: " << processes[i]->burst_time << endl;
		
	}*/
	
    return 0;
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
	return argv[1];
}

// parse the commandline for the schedule type
char* parse_sched(char** argv) {
	return argv[2];
}

// parse the commandline for the quantum number
int parse_quantum(char** argv) {
	return atoi(argv[3]);
}

// Read the given input file into a useable format
bool input_file(char* filename, process_struct** processes) {
	
	// set up
	ifstream input("input1.txt");
	string line;
	string temp_string;
	int array_index = 0;
	int variable_index = 0;
	int store_row[3];
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
	
	// for testing output, may need later
	/*
	for (int i = 0; i < 6; i++) {
		
		cout << "TEMP[" << i << "] PID: " << processes[i]->pid << "\t";
		cout << "TEMP[" << i << "] START: " << processes[i]->arrival_time << "\t";
		cout << "TEMP[" << i << "] BURST: " << processes[i]->burst_time << endl;
		
	}
	*/
	cout << endl << endl;
	return true;
}
