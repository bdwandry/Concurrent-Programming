/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/4/2021                                        */
/* PROGRAM ASSIGNMENT 4                                        */
/* FILE NAME : thread.h                                        */
/* PROGRAM PURPOSE : The purpose of this program is to solve   */
/* the Party Problem by implementing 2 different types of      */
/* threads, students and landlord. Students                    */
/* will join/leave a room freely. However, if the landlord     */
/* appears at a random time and the room is over capacity, they*/
/* will remove everyone from that room. This file contains     */
/* starting logic for both the Students and the Landlord.      */
/* ----------------------------------------------------------- */

#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

int StudentsThread::studentsInRoom = 0;
int StudentsThread::totalProcesses = 0;
int LandLordThread::completed = 0;
Semaphore ExSem("Exit", 1);
Semaphore EntSem("Enter", 1);
Mutex PtrLck("Printing");

/* ----------------------------------------------------------- */
/* FUNCTION: StudentsThread                                    */
/*    This is the constructor to the StudentsThread class      */
/*    described in the thread.h. It will supply definitions    */
/*    for pass through arguments passed from main.             */
/* PARAMETER USAGE :                                           */
/*    int i = Passed from Main of the index for each           */
/*	  specificfied student thread.                             */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
StudentsThread::StudentsThread(int i) {
	studentID = i;
}

/* ----------------------------------------------------------- */
/* FUNCTION: LandLordThread                                    */
/*    This is the constructor to the LandLordThread class      */
/*    described in the thread.h. It will supply definitions    */
/*    for pass through arguments passed from main.             */
/* PARAMETER USAGE :                                           */
/*    int m - used for number of times the landlord checks the */
/*            room                                             */
/*    int n - Keeps track of the rooms capacity                */
/* FUNCTION CALLED :                                           */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
LandLordThread::LandLordThread(int m, int n) {
	numOfAptmSearches = m;
	capacity = n;
	iteration = 0;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Student ThreadFunc                                */
/*    This is the executing code for the specific thread       */
/*	  created. It will call the GoToParty function for the     */
/*    Students to enter in the room                            */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTION CALLED :                                           */
/*    ThreadMentor                                             */
/*    Main                                                     */
/* ----------------------------------------------------------- */
void StudentsThread::ThreadFunc() {
	Thread::ThreadFunc();
	PtrLck.Lock();
		printf("     Student %d starts\n", studentID);
	PtrLck.Unlock();
	while(LandLordThread::completed != 1) {
		Delay();
		GoToParty();
		Delay();
	}		
	
	PtrLck.Lock();
		printf("     Student %d terminates\n", studentID + 1);
		StudentsThread::totalProcesses--;
	PtrLck.Unlock();
	Exit();
}

/* ----------------------------------------------------------- */
/* FUNCTION: Landlord ThreadFunc                               */
/*    This is the executing code for the specific thread       */
/*	  created. It will call the CheckRoom function for the     */
/*    Landlord to enter in the room and see if its over        */
/*    capacity                                                 */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTION CALLED :                                           */
/*    ThreadMentor                                             */
/*    Main                                                     */
/* ----------------------------------------------------------- */
void LandLordThread::ThreadFunc() {
	Thread::ThreadFunc();
	for (int i = 0; i < numOfAptmSearches; i++) {
		iteration = i;
		Delay();
		CheckRoom();
		//printf("Landlord: %d/%d; Capacity: %d\n", i+1, numOfAptmSearches, capacity);
		if ((i + 1) == numOfAptmSearches) {
			LandLordThread::completed = 1;
			while(StudentsThread::totalProcesses != 0) {
				Delay();
			}
			break;
		}
		Delay();
	}
	
	PtrLck.Lock();
		printf("After checking the room %d times, the landlord retires and is on vacation!\n", numOfAptmSearches);
	PtrLck.Unlock();
	
	Exit();
}

