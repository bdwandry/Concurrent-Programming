/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/13/2021                                       */
/* PROGRAM ASSIGNMENT 6                                        */
/* FILE NAME : thread.h                                        */
/* PROGRAM PURPOSE : The purpose of this program is to         */
/* solve/generate n number of primes using the Sieves method.  */
/* This program will only use one global variable and          */
/* thread-based channels to complete the overarching program.  */
/* ----------------------------------------------------------- */
#include "ThreadClass.h"

//Global Definitions defined by header
extern int *Primes;
const int END_OF_DATA = -1; 

/* ----------------------------------------------------------- */
/* FUNCTION: PrimeCheck (Class Definition)                     */
/*    This is the header file that describes the threadmentor's*/
/*	  class. This class will be shared with all files when	   */
/*	  threads are called for Prime Checking based threads.     */
/* PARAMETER USAGE :                                           */
/*    int index - The number to be passed along.               */
/*	  int ThreadID - The identification number for Each Thread */
/*    SynOneToOneChannel *beforeChan - The previous Thread     */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class PrimeCheck : public Thread {
  public:
    PrimeCheck(int index, int ThreadID, SynOneToOneChannel *beforeChan);
	int PrintPrime(char * buffer);
	int CurrentThreadID();
  private:
    int Index;int neighbor;
	SynOneToOneChannel *beforeChan;
	SynOneToOneChannel *afterChan;
    PrimeCheck *primecheck;
    void ThreadFunc();
};

/* ----------------------------------------------------------- */
/* FUNCTION: MasterThread (Class Definition)                   */
/*    This is the header file that describes the threadmentor's*/
/*	  class. This class will be shared with all files when	   */
/*	  threads are called for Master based threads.             */
/* PARAMETER USAGE :                                           */
/*    int n - The upper bound to thread checking limit         */
/*	  int ThreadID - The identification number for Each Thread */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class MasterThread : public Thread {
  public:
    MasterThread(int ThreadID, int n);
	int PrintMaster(char * buffer);
  private:
    int n; int TotalNumberOfPrimes;
	SynOneToOneChannel *beforeChan;
	SynOneToOneChannel *afterChan;
    PrimeCheck *primecheck;
    void ThreadFunc();
};

