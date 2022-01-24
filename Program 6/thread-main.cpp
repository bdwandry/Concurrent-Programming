/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/13/2021                                       */
/* PROGRAM ASSIGNMENT 6                                        */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE : The purpose of this program is to         */
/* solve/generate n number of primes using the Sieves method.  */
/* This program will only use one global variable and          */
/* thread-based channels to complete the overarching program.  */
/* ----------------------------------------------------------- */
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

//Global Primes array initialization
int * Primes;

/* ----------------------------------------------------------- */
/* FUNCTION: SpawnThreads                                      */
/*    This is the function that will spawn the Master thread.  */
/*	  In the Masterthread logic, it will spawn the PrimeCheck  */
/*	  threads accordingly.									   */
/* PARAMETER USAGE :                                           */
/*    int n = Passed from Main that indicates the upper bound  */
/*	  for prime numbers to be generated.					   */
/* FUNCTION CALLED:                                            */
/*    Main                                                     */
/* ----------------------------------------------------------- */
int SpawnThreads(int n) {
	//Spawn Master Thread
	MasterThread *master = new MasterThread(0, n);
	master->Begin();
	master->Join();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Main                                              */
/*    This is the starting point of the program. It will start */
/*    out by checking a couple of initial conditions and will  */
/*	  instantiate n from the argument                          */ 
/* PARAMETER USAGE :                                           */
/*    Argv[1] = Passed from arguments from the command line    */
/*	  that indicates the upper bound for prime numbers to be   */
/*	  generated.					                           */
/* FUNCTION CALLED:                                            */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	//This is the Initial Condition Section
	int n;
	if (argc == 1) {
		n = 30; 
	} else if(argc > 2) {
		printf("ERROR: There is only aloud to have at max one argument.\n");
		return -1;
	} else if (atoi(argv[1]) < 3) {
		printf("ERROR: N must be greater than or equal to 3\n");
		return -1;
	} else {
		n = atoi(argv[1]);
	} 
	
	Primes = (int *) malloc(sizeof(int) * n);
	
	for (int i = 0; i < n; i++) {
		Primes[i] = -1;
	}
	
	SpawnThreads(n);

	return 1;
}
