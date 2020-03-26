Authors: Jack Gularte and Anthony Caballero
Date: February 17th 2019
Class: CPSC 3500
Assignment: Project Three
To Compile: make p3
To Run: ./p3

// contributions
Jack: created/designed main; did logging funcitons
Tony: Helped with main, created member functions

// about the script
Uses three threads; one that simulates the 'worker', one that creates cars from the north,
and a thread that creates car coming from the south. 

//Semaphore Use
Uses two semaphores in order to keep synchronization correct. One is used to force threads to
enter the critical section while the other is used in order to keep order in adding times to the vector.

//Mutex Lock Use
Uses two mutex locks to avoid all race conditions located in the critical section as well as accessing
the vector of times.

//Strengths
Successfuly logs the cars' travel information into a file.
Successfuly logs the worker's information into a file. 
Maintains concurency while also not falling victim to deadlocks

//Weaknesses
Output files may be difficult to read at some points
Upon running the project, a segmentation fault may occur right at the beginning. 
Many debugging attempts were made but we were not successful in finding the issue. 
If this occurs, simply run again and the program will run correctly. 