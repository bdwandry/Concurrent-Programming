/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/4/2021                                        */
/* PROGRAM ASSIGNMENT 4                                        */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE : The purpose of this program is to solve   */
/* the Party Problem by implementing 2 different types of      */
/* threads, students and landlord. Students                    */
/* will join/leave a room freely. However, if the landlord     */
/* appears at a random time and the room is over capacity, they*/
/* will remove everyone from that room. This file will act     */
/* as the starting point for the rest of the program, it will  */
/* spawn the threads accordingly.                              */
/* ----------------------------------------------------------- */
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

/* ----------------------------------------------------------- */
/* FUNCTION: spawnThreads                                      */
/*    The pourpose of this function is to spawn the threads    */
/*    for the Students and Landlord              			   */
/* PARAMETER USAGE :                                           */
/*    int m: number of times the landlord checks the room.     */
/*    int n: maximum number of students allowed in the room.   */
/*    int k: total number of students                          */
/* FUNCTION CALLED :                                           */
/*    Main                                                     */
/* ----------------------------------------------------------- */
int spawnThreads(int m, int n, int k) {
	//Initializer for the threads
	LandLordThread *landlordThread = new LandLordThread(m, n);
	StudentsThread *studentsThread[k];
	
	//Spawns the Landlord Thread
	landlordThread->Begin();
	
	//Spawns Student Threads
	for (int i = 0; i < k; i++) {
		StudentsThread::totalProcesses++;
		studentsThread[i] = new StudentsThread(i);
		studentsThread[i]->Begin();
	}
	
	
	//Waits for all the students threads to exit gracefully
	for (int i = 0; i < k; i++) {
		studentsThread[i]->Join();
	}
	
	//Waits for the landlord thread to exit gracefully
	landlordThread->Join();
	
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: main                                              */
/*    This function is the main function to this program       */
/*    It will start out by reading inputs from arguments and   */
/*    spawn later process off of those inputs.                 */
/* PARAMETER USAGE :                                           */
/*    Uses in Argv[1...3]                                      */
/* FUNCTION CALLED :                                           */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	//Initial Argument Checking
	if (argc != 4) {
		printf("ERROR: ./prog4 m n k\n");
		return -1;
	}
	
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int k = atoi(argv[3]);
	
	if (m == 0) {
		m = 5;
	}
	
	if (n == 0) {
		n = 5;
	}
	
	if (k == 0) {
		k = 10;
	}
	
	//Spawns the landlord and student Threads
	spawnThreads(m, n, k);
	return 1;
}