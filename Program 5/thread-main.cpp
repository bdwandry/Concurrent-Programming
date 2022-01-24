/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/17/2021                                       */
/* PROGRAM ASSIGNMENT 5                                        */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE : The purpose of this program is to         */
/* solve the Santa, Elves, and Reindeer problem described      */
/* by the assignment 5 page. This is only aloud to use         */ 
/* Monitors. Unfortunitely, this program will recieve DeadLock.*/
/* ----------------------------------------------------------- */
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

/* ----------------------------------------------------------- */
/* FUNCTION: SpawnThreads                                      */
/*    This is the function that will spawn the Master thread.  */
/*	  This will spawn the Santa, Reindeer, and Elves Threads   */
/*	  accordingly.                                             */
/* PARAMETER USAGE :                                           */
/*    int e = number of elves            					   */
/*    int r = number of Reindeer          					   */
/*    int t = number of toys            					   */
/* FUNCTION CALLED:                                            */
/*    Main                                                     */
/* ----------------------------------------------------------- */
int SpawnThreads(int e, int r, int t) {
	Mon *mon = new Mon(e, r);
	Santa *santa = new Santa(t, mon);
	santa->Begin();

	//Starting Reindeer
	Reindeer *deer[r];
	for (int i = 0; i < r; i++) {
		deer[i] = new Reindeer(i, mon);
		deer[i]->Begin();
	}
	
	//Starting Elves
	Elves *elves[e];
	for (int i = 0; i < e; i++) {
		elves[i] = new Elves(i, mon);
		elves[i]->Begin();
	}
	
	//------------------------------------------
	//Waits for Reindeer
	for (int i = 0; i < r; i++) {
		deer[i]->Join();
	}
	//Waits for elves
	for (int i = 0; i < e; i++) {
		elves[i]->Join();
	}

	//waits for santa
	santa->Join();

	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Main                                              */
/*    This is the starting point of the program. It will start */
/*    out by checking a couple of initial conditions and will  */
/*	  instantiate e, r, & t from the argument.                 */ 
/* PARAMETER USAGE :                                           */
/*    Argv[1] = equals e = number of elves                     */
/*    Argv[2] = equals r = number of reindeer                  */
/*    Argv[3] = equals t = number of toys                      */
/* FUNCTION CALLED:                                            */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("ERROR: ./prog5 e r t\n");
		return -1;
	}
	
	//Checking for non-negative numbers
	if ((atoi(argv[1]) < 0) || (atoi(argv[2]) < 0) || (atoi(argv[3]) < 0)) {
		printf("ERROR: e, r, and t must be non-negative numbers.\n");
		return -1;
	}
	
	//Initial Condition Setup
	int e = atoi(argv[1]);
	int r = atoi(argv[2]);
	int t = atoi(argv[3]);
	
	//Inititial variable checking
	if (e == 0) {
		e = 7;
	}
	
	if (r == 0) {
		r = 9;
	}
	
	if (t == 0) {
		t = 5;
	}

	SpawnThreads(e, r, t);
	return 1;
}
