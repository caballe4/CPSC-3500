//Jack Gularte and Anthony Caballero


#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <fstream>
#include <iostream>

#define MAX_CARS 40
#define MAX_WAIT 10
#define PERCENT 8

using namespace std;

// log filenames
string worker_file = "flagperson.log";
string car_file = "car.log";

// logging functions
void worker_header();
void car_header();
void worker_log(string message);
void car_log(string message); 
void car_log_ID(int id);
string getTime();
void car_log_message(string message);


// function prototypes
int pthread_sleep (int seconds);
void *worker(void *arg);
void *createCar(void *arg);	

//semaphores and mutexes
sem_t work;
pthread_mutex_t worker_mutex;
sem_t car;
pthread_mutex_t car_mutex;

// global variables
vector<string> times;
int direction = 0;
int north_wait = 0;
int south_wait = 0;
bool flag = true;

int main()
{

	// initialize sem/mut
	sem_init(&work, 0, 0);
	pthread_mutex_init(&worker_mutex, NULL);
	
	sem_init(&car, 0, 1);
	pthread_mutex_init(&car_mutex, NULL);
	
	// headers
	worker_header();
	car_header();
	
	// Create the thread for flagger
	pthread_t worker_id;
	
	if (pthread_create(&worker_id, NULL, &worker, NULL) == -1 )
	{
		perror("Error: Could not create the 'Flagger' thread");
		return -1;
	}	
	
	// Create the thread for both directions of cars,
	// north and south
	char direction_one = 'N';
	pthread_t car_north;
	if(pthread_create(&car_north, NULL, &createCar, (void*)&direction_one) == -1)
	{
		perror("Error: Could not create the 'Car_north' thread");
		return -1;
	}
	
	char direction_two = 'S';
	pthread_t car_south;
	if(pthread_create(&car_south, NULL, &createCar, (void*)&direction_two) == -1)
	{
		perror("Error: Could not create the 'Car_south' thread");
		return -1;
	}
	
	// rejoin and cancel
	pthread_join(worker_id, NULL);
	pthread_cancel(car_south);
	pthread_cancel(car_north);
	
	// destroy
	sem_destroy(&work);
	pthread_mutex_destroy(&worker_mutex);
	sem_destroy(&car);
	pthread_mutex_destroy(&car_mutex);
	
	return 0;
}

void *createCar(void *arg)
{
	int count = 0;
	char* direction2 = (char*)arg;
	
	while(1){
		//seeding random number generator
		srand(time(NULL) + count);
		
		//80% chance a car follows
		if(rand()%10 +1 <= PERCENT){
			sem_wait(&car);
			pthread_mutex_lock(&car_mutex);
			if(*direction2 == 'N')
				north_wait++;
			else
				south_wait++;
			//put arrival time at end of vector
			times.push_back(getTime());
			//update semaphores
			pthread_mutex_unlock(&car_mutex);
			sem_post(&car);
		}else{
			//20 second delay for no car
			pthread_sleep(10);
		}
		count++;
	}
	return NULL;
}

int pthread_sleep (int seconds)
{
	pthread_mutex_t mutex;
	pthread_cond_t conditionvar;
	struct timespec timetoexpire;
	if(pthread_mutex_init(&mutex,NULL))
	{
		return -1;
	}
	if(pthread_cond_init(&conditionvar,NULL))
	{
		return -1;
	}
	
	//When to expire is an absolute time, so get the current time and add
	 //it to our delay time
	timetoexpire.tv_sec = (unsigned int)time(NULL) + seconds;
	timetoexpire.tv_nsec = 0;
	return pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
}

// functions for logging
void worker_header() {
	ofstream work_stream(worker_file.c_str(), ios_base::out | ios_base::app);
	work_stream << "Time \t\t State \n";
	work_stream.close();
}

void car_header() {
	ofstream car_stream(car_file.c_str(), ios_base::out | ios_base::app);
	car_stream << "CarID \t Direction \t Arrival-Time \t Start-Time \t End-Time" << endl;
	car_stream.close();
}

void worker_log(string message) {
	string time = getTime(); 
    ofstream work_stream(worker_file.c_str(), ios_base::out | ios_base::app );
    work_stream << time << "\t" << message << endl;
    work_stream.close();
}

void car_log_time()
{
	string time = getTime();
	ofstream car_stream(car_file.c_str(), ios_base::out | ios_base::app);
	car_stream << time << "\t";
	car_stream.close();
}

void car_log_ID(int id)
{
	ofstream car_stream(car_file.c_str(), ios_base::out | ios_base::app);
	car_stream << id << "\t";
	car_stream.close();
}

void car_log_message(string message)
{
	ofstream car_stream(car_file.c_str(), ios_base::out | ios_base::app);
	car_stream << message << "\t";
	car_stream.close();
}

string getTime()
{
	string time;
	time_t var;
	char arr[MAX_CARS];
	var = NULL;
	struct tm *local_time = localtime(&var);
	strftime(arr, MAX_CARS, "%H:%M:%S", local_time);
	time = arr;
	return time;
}
	
void *worker(void *arg)
{
	
	int count = 0;
	//int cars = 0;
	char car_direction;
	while(count < MAX_CARS)
	{
		//synchronization for no deadlocks. Checks for first thread to get lock
		if(!flag){
			sem_wait(&work);
			pthread_mutex_lock(&worker_mutex);
		}else{
			flag = false;
			pthread_mutex_lock(&worker_mutex);
		}
		
		//no cars in either direction, sleep
		if(north_wait == 0 && south_wait == 0){
			worker_log("sleep");
			cout << "No cars: sleeping..." << endl;
			//wait until car arrives
			while(north_wait == 0 || south_wait == 0);
			cout << "Car arrived" << endl; 
			worker_log("Awake");
			
			//Check directions
			if(north_wait > south_wait)
				direction = 1;
			else
				direction = 0;
		}
		
		// set the direction of the car to go
		if((direction == 0 && north_wait < MAX_WAIT))
			direction = 0;
		else if(direction == 1 && south_wait < MAX_WAIT)
			direction = 1;	
		// switch directions if current direction is empty
		else if(direction == 0 && south_wait <= 0 && north_wait > 0)
			direction = 1;
		else if (direction == 1 && north_wait <= 0 && south_wait > 0)
			direction = 0;
		
		if(direction == 1)
			car_direction = 'N';
		else
			car_direction = 'S';
		
		cout << "SOUTH_WAIT: " << south_wait << "\t NORTH_WAIT: " << north_wait << "\t DIRECTION: " << car_direction << endl;
		if(car_direction == 'S' && south_wait > 0){
			string arrival = times.front();
			times.pop_back();
			
			//log arrival time in file
			car_log_ID(count);
			car_log_message("\t S \t");
			car_log_message(arrival);
			car_log_time();
			
			//decrement south waiting time
			south_wait--;
			count++;
			cout << "Car ID: " << count << " Direction: " << car_direction << endl;
			pthread_sleep(2);
			cout << "Car ID: " << count << " exiting" << endl;
			
			//logging exiting time 
			car_log_time();
			
			ofstream car_stream(car_file.c_str(), std::ios_base::out | std::ios_base::app );
			car_stream << endl;
			car_stream.close();
			
			cout << "Number of Cars Remaining" << endl;
			cout << "North: " << north_wait << "\t" << "South: " << south_wait << endl;
			
		}else if(car_direction == 'N' && north_wait > 0){
			
			cout << "HELLO AM I EVER CALLED" << endl;
			string arrival = times.front();
			times.pop_back();
			
			//log arrival time in file
			car_log_ID(count);
			car_log_message("\t N \t");
			car_log_message(arrival);
			car_log_time();
			
			//decrement south waiting time
			north_wait--;
			count++;
			cout << "Car ID: " << count << " Direction: " << car_direction << endl;
			pthread_sleep(2);
			cout << "Car ID: " << count << " exiting" << endl;
			
			//logging exiting time 
			car_log_time();
			ofstream car_stream(car_file.c_str(), std::ios_base::out | std::ios_base::app );
			car_stream << endl;
			car_stream.close();
			
			cout << "Number of Cars Remaining" << endl;
			cout << "North: " << north_wait << "\t" << "South: " << south_wait;
		}
		else {
			pthread_sleep(2);
			cout << "ELSE" << endl;
		}
		
		//done with semaphores. Update and unlock them
		pthread_mutex_unlock(&worker_mutex);
		sem_post(&work);
		
	}
	
	return NULL;
}