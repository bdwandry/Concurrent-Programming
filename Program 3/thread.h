/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 11/3/2021                                        */
/* PROGRAM ASSIGNMENT 3                                        */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE : To conduct an sorting algorithm based on  */
/* even-odd paradigm comcurrently. This files function is to   */
/* create a link and shared memory between the threads. As well*/
/* as creating a link to threadmentor in the backend.          */
/* ----------------------------------------------------------- */
#include "ThreadClass.h"

//Global variables that share values between Main and thread.cpp class
extern int arrSize; 
extern int * arrData;

/* ----------------------------------------------------------- */
/* FUNCTION: EvenOddThread (Class Definition)                  */
/*    This is the header file that describes the threadmentor's*/
/*	  class. This class will be shared with all files when	   */
/*	  threads are called.                                      */
/* PARAMETER USAGE :                                           */
/*    int in - used to pass through k                          */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class EvenOddThread : public Thread {
 	public: 
		EvenOddThread(int in);
		
	private: 
		int index;
		void ThreadFunc();
};