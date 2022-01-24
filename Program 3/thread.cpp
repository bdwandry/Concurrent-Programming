/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 11/3/2021                                        */
/* PROGRAM ASSIGNMENT 3                                        */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE : To conduct an sorting algorithm based on  */
/* even-odd paradigm comcurrently. This files function, in     */
/* separate threads, to do the array swapping if the conditions*/
/* are met if x[k-1] is less than x[k]. This will run          */
/* threadmentor in the backend for thread-management.          */
/* ----------------------------------------------------------- */
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* ----------------------------------------------------------- */
/* FUNCTION: EvenOddThread                                     */
/*    This is the constructor to the EvenOddThread class       */
/*    described in the thread.h. It will supply definitions    */
/*    for pass through arguments passed from main.             */
/* PARAMETER USAGE :                                           */
/*    int in = Passed from Main of the index for each          */
/*	  specificfied thread.                                     */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
EvenOddThread::EvenOddThread(int in) {
	index = in;
}

/* ----------------------------------------------------------- */
/* FUNCTION: ThreadFunc                                        */
/*    This is the executing code for the specific thread       */
/*	  created. It will check the indices of the input array    */
/*    to see if x[x-1] > x[x], then swap, or else do nothing   */
/* PARAMETER USAGE :                                           */
/*    ThreadMentor - used for managing threads                 */
/*    index - defined from the constructor                     */
/* FUNCTION CALLED :                                           */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
void EvenOddThread::ThreadFunc() {
	Thread::ThreadFunc();
	Delay();
	//Print outs
	char threadBuf[10000];
	sprintf(threadBuf, "        Thread %d Created\n", index);
	write(1, threadBuf, strlen(threadBuf));		
	memset(threadBuf, 0, 10000);
	Delay();	
	
	sprintf(threadBuf, "        Thread %d compares %d and %d\n", index, arrData[index - 1], arrData[index]);
	write(1, threadBuf, strlen(threadBuf));		
	memset(threadBuf, 0, 10000);
	Delay();
	
	//Does the swapping
	if (arrData[index - 1] > arrData[index]) {
		Delay();
		sprintf(threadBuf, "        Thread %d swaps %d and %d\n", index, arrData[index - 1], arrData[index]);
		write(1, threadBuf, strlen(threadBuf));		
		memset(threadBuf, 0, 10000);
		Delay();

		int temp = arrData[index];
		arrData[index] = arrData[index - 1];
		arrData[index - 1] = temp;
		Delay();
	}
	
	//Print outs
	sprintf(threadBuf, "        Thread %d exits\n", index);
	write(1, threadBuf, strlen(threadBuf));		
	memset(threadBuf, 0, 10000);
	Delay();
	Exit();
}
