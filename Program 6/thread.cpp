/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/13/2021                                       */
/* PROGRAM ASSIGNMENT 6                                        */
/* FILE NAME : thread.cpp                                      */
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

/* ----------------------------------------------------------- */
/* FUNCTION: PrimeCheck                                        */
/*    This is the constructor to the PrimeCheck class          */
/*    described in the thread.h. It will supply definitions    */
/*    for pass through arguments passed from main.             */
/* PARAMETER USAGE :                                           */
/*    int index - The number to be passed along.               */
/*	  int ThreadID - The identification number for Each Thread */
/*    SynOneToOneChannel *beforeChan - The previous Thread     */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
PrimeCheck::PrimeCheck(int index, int ThreadID, SynOneToOneChannel *beforeChan) : beforeChan(beforeChan) {
	Index = index;
	neighbor = false;
	UserDefinedThreadID = ThreadID;
	afterChan = new SynOneToOneChannel("Prime", ThreadID, ThreadID + 1);
}

/* ----------------------------------------------------------- */
/* FUNCTION: PrintPrime                                        */
/*    This class is to simplify the printing experience for    */ 
/*    the PrimeCheck threads. It will be used a myriad of      */
/*    different times throughout a Prime Checking Threads.     */
/* PARAMETER USAGE :                                           */
/*    char * buf - This is a pass through buffer pointer       */
/*    that will be used in printing.                           */
/* FUNCTION CALLED:                                            */
/*    PrimeCheck(...)                                          */
/* ----------------------------------------------------------- */
int PrimeCheck::PrintPrime(char * buf) {
	char writeBuffer[10000];
	sprintf(writeBuffer, buf);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: CurrentThreadID                                   */
/*    This function will be unique to every instantiated       */
/*    Thread. It will will return the ThreadID the thread      */
/*	  that calls this function.                                */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTION CALLED:                                            */
/*    PrimeCheck(...)                                          */
/* ----------------------------------------------------------- */
int PrimeCheck::CurrentThreadID() {
	return Index;
}

/* ----------------------------------------------------------- */
/* FUNCTION: PrimeCheck::ThreadFunc                            */
/*    This will be the portion that will do the checking if    */
/*    if a number is going to be prime or not.                 */
/* PARAMETER USAGE :                                           */
/*    MasterThread::ThreadFunc() - A number passed from master */
/*    will be checked if it is prime or not                    */
/* FUNCTION CALLED :                                           */
/*	  MasterThread::ThreadFunc() (Indirectly - Using Channels) */
/*    ThreadMentor                                             */
/*    Main                                                     */
/* ----------------------------------------------------------- */
void PrimeCheck::ThreadFunc() {
	Thread::ThreadFunc();
	char writeBuffer[10000];
	int CurrentThread = 0;
	sprintf(writeBuffer, "%*cP%d starts and memorizes %d\n", (Index + 1), ' ', Index, Index);
	PrintPrime(writeBuffer);
	while (1) {
		beforeChan->Receive(&CurrentThread, sizeof(int));
		if (CurrentThread == END_OF_DATA) {
			break;
		}		
		sprintf(writeBuffer, "%*cP%d receives %d\n", (Index + 1), ' ', Index, CurrentThread);
		PrintPrime(writeBuffer);

		if (CurrentThread % Index == 0) {
			sprintf(writeBuffer, "%*cP%d ignores %d\n", (Index + 1), ' ', Index, CurrentThread);
			PrintPrime(writeBuffer);
		} else if (neighbor == 1){
			sprintf(writeBuffer, "%*cP%d sends %d to P%d\n", (Index + 1), ' ', Index, CurrentThread, primecheck->CurrentThreadID());
			PrintPrime(writeBuffer);
			afterChan->Send(&CurrentThread, sizeof(int));
		} else {
			neighbor = 1;
			sprintf(writeBuffer, "%*cP%d creates P%d\n", (Index + 1), ' ', Index, CurrentThread);
			PrintPrime(writeBuffer);
			primecheck = new PrimeCheck(CurrentThread, UserDefinedThreadID + 1, afterChan);
			primecheck->Begin();
		}
	}

	sprintf(writeBuffer, "%*cP%d receives END\n", (Index + 1), ' ', Index);
	PrintPrime(writeBuffer);
	if (neighbor == 1) {
		CurrentThread = END_OF_DATA;
		afterChan->Send(&CurrentThread, sizeof(int));
		primecheck->Join();
	}
	Primes[UserDefinedThreadID - 1] = Index;
	Exit();
}

//---------------------------------------------------------------------------------------------------------------------------------//
/* ----------------------------------------------------------- */
/* FUNCTION: MasterThread                                      */
/*    This is the constructor to the MasterThread class        */
/*    described in the thread.h. It will supply definitions    */
/*    for pass through arguments passed from main.             */
/* PARAMETER USAGE :                                           */
/*    int n - The upper bound to thread checking limit         */
/*	  int ThreadID - The identification number for Each Thread */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
MasterThread::MasterThread(int ThreadID, int n) {
	TotalNumberOfPrimes = n;
	UserDefinedThreadID = ThreadID;
	afterChan = new SynOneToOneChannel("Master", UserDefinedThreadID, UserDefinedThreadID + 1);
}

/* ----------------------------------------------------------- */
/* FUNCTION: PrintMaster                                       */
/*    This class is to simplify the printing experience for    */ 
/*    the PrimeCheck threads. It will be used a myriad of      */
/*    different times throughout a Master Threads.	           */
/* PARAMETER USAGE :                                           */
/*    char * buf - This is a pass through buffer pointer       */
/*    that will be used in printing.                           */
/* FUNCTION CALLED:                                            */
/*    MasterThread(...)                                        */
/* ----------------------------------------------------------- */
int MasterThread::PrintMaster(char * buf) {
	char writeBuffer[10000];
	sprintf(writeBuffer, buf);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: MasterThread::ThreadFunc                          */
/*    This is a thread that will only be used to pass numbers  */
/*    3 to n to their respected PrimeCheck Threads.            */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTION CALLED :                                           */
/*    ThreadMentor                                             */
/*    Main                                                     */
/* ----------------------------------------------------------- */
void MasterThread::ThreadFunc() {
	Thread::ThreadFunc();
	char writeBuffer[10000];
	PrintMaster("Master starts\n");
	primecheck = new PrimeCheck(2, UserDefinedThreadID + 1, afterChan);
	primecheck->Begin();
	int input = 3;
	do {
		if (input - 1 == TotalNumberOfPrimes) {
			PrintMaster("Master sends END\n");
			input = END_OF_DATA;
			afterChan->Send(&input, sizeof(int));
			break;
		} else {
			sprintf(writeBuffer, "Master sends %d to P2\n", input);
			PrintMaster(writeBuffer);
			afterChan->Send(&input, sizeof(int));
		}
		input++;
	} while (input != END_OF_DATA);

	primecheck->Join();
	PrintMaster("Master prints the complete result:\n");

	for (int i = 0; i < TotalNumberOfPrimes; i++) {
		if (i == 0) {
			PrintMaster("  ");
		}
		if (Primes[i] == -1) {
			PrintMaster("\n");
			break;	
		} else {
			sprintf(writeBuffer, "%d ", Primes[i]);
			PrintMaster(writeBuffer);
		}
	}
	
	PrintMaster("Master terminates\n");
	Exit();
}