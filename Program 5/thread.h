/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/17/2021                                       */
/* PROGRAM ASSIGNMENT 5                                        */
/* FILE NAME : thread.h                                        */
/* PROGRAM PURPOSE : The purpose of this program is to         */
/* solve the Santa, Elves, and Reindeer problem described      */
/* by the assignment 5 page. This is only aloud to use         */ 
/* Monitors. Unfortunitely, this program will recieve DeadLock.*/
/* ----------------------------------------------------------- */
#include "ThreadClass.h"

const int REINDEEREV = 0;
const int ELVEDEV = 1;
class Reindeer;
class Elves;

/* ----------------------------------------------------------- */
/* FUNCTION: Mon (Class Definition for Monitor)                */
/*    This is a class definition for the monitor used 		   */
/*    throughout this program                                  */
/* PARAMETER USAGE :                                           */
/*    int e - The total number of elves                        */
/*	  int r - The total number of reindeer                     */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    main                                                     */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class Mon : public Monitor {
  public:
	//Program Defined Monitor Procedures
    Mon(int e, int r);
    int AskQuestion(Elves *elves);
    int ReindeerBack(Reindeer *reindeer);
    int WaitOthers(Reindeer *reindeer);
    int WaitSleigh(Reindeer *reindeer);
    int FlyOff(Reindeer *reindeer);
    int Sleep();
	
	//User Defined Monitor Procedures
	int RetSanta();
    int Retired();
    int ElvesRequest();
    int ReindeerRequest(int numOfTimes);
	int PrintText(char * buf);

  private:
    int retiredSanta; int used;
    int TotalNumberOfElves; int TotalNumberOfReindeer; int currentNumOfElves; int currentNumOfReindeer;
    Elves **eArr;
	Elves *previousElves;
    Reindeer *previousReindeer;
    Condition *Wake; Condition *CompleteSantaTask; Condition *WaitElves; Condition *WaitDeer;
	
};

/* ----------------------------------------------------------- */
/* FUNCTION: Reindeer (Class Definition for Monitor)           */
/*    This is a class definition for the reindeer thread       */
/* PARAMETER USAGE :                                           */
/*    int ThreadID - The i'th reindeer thread creation         */
/*	  Mon mon - A pointer to the monitor created               */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    main                                                     */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class Reindeer : public Thread {
  public:
    Reindeer(int threadId, Mon *mon);
    int GetReindeerID();

  private:
    int ReindeerID;
    Mon *mon; Mon *mymon;
    void ThreadFunc();
};

/* ----------------------------------------------------------- */
/* FUNCTION: Elves (Class Definition for Monitor)              */
/*    This is a class definition for the elves thread          */
/* PARAMETER USAGE :                                           */
/*    int ThreadID - The i'th elves thread created             */
/*	  Mon mon - A pointer to the monitor created               */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    main                                                     */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class Elves : public Thread {
  public:
    Elves(int ThreadID, Mon *mon);
    int GetElvesID();

  private:
    int ElvesID;
    Mon *mon; Mon *mymon;
    void ThreadFunc();
};

/* ----------------------------------------------------------- */
/* FUNCTION: Santa (Class Definition for Monitor)              */
/*    This is a class definition for the elves thread          */
/* PARAMETER USAGE :                                           */
/*    int t - The total number of toys                         */
/*	  Mon mon - A pointer to the monitor created               */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    main                                                     */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class Santa : public Thread {
  public:
    Santa(int t, Mon*);

  private:
    int t; int TotalNumOfToys;
    Mon *mon; Mon *mymon;
    void ThreadFunc();
};
