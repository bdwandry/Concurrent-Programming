/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 12/17/2021                                       */
/* PROGRAM ASSIGNMENT 5                                        */
/* FILE NAME : thread.cpp                                      */
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
/* FUNCTION: Mon::Mon                                          */
/*    This is the constructor for the HOARE Monitor being used */
/* PARAMETER USAGE :                                           */
/*    int e - Total number of elves                            */
/*	  int r - Total number of reindeer                         */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
Mon::Mon(int e, int r) : Monitor("Monitor", HOARE) {
	used = 0;
	currentNumOfElves = 0;
	currentNumOfReindeer = 0;
	retiredSanta = 0;
	TotalNumberOfElves = e;
	TotalNumberOfReindeer = r;
	eArr = (Elves**) malloc(sizeof(Elves*) * TotalNumberOfElves);
	Wake = new Condition("Wake up");
	CompleteSantaTask = new Condition("Santa's Woken Task");
	WaitElves = new Condition("Elves are waiting");
	WaitDeer = new Condition("Reindeer are waiting");
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::AskQuestion (Monitor Procedure)              */
/*    This is a monitor procedure for when elves ask questions.*/
/*    The logic for them is laid out in this function.         */
/* PARAMETER USAGE :                                           */
/*    elves - Pass along an elve object and get current info.  */
/* FUNCTION CALLED:                                            */
/*    Elves::ThreadFunc()                                      */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::AskQuestion(Elves *elves) {
	MonitorBegin();
	char WriteBuffer[10000];
	sprintf(WriteBuffer, "         Elf %d has a problem\n", elves->GetElvesID() + 1);
	PrintText(WriteBuffer);
	eArr[currentNumOfElves] = elves;
	currentNumOfElves++;
	
	if (currentNumOfElves > 3) {
		CompleteSantaTask->Wait();
		WaitElves->Wait();
	}
	
	if (currentNumOfElves == 3 ) { 
		sprintf(WriteBuffer, "         Elves %d, %d, %d wake up Santa\n", eArr[0]->GetElvesID() + 1, eArr[1]->GetElvesID() + 1, eArr[2]->GetElvesID() + 1);
		PrintText(WriteBuffer);
		previousElves = elves;
		Wake->Signal();
	} else {
		WaitElves->Wait();
	}
	
	if (elves == previousElves || used) {
		CompleteSantaTask->Signal();	
		sprintf(WriteBuffer, "         Elves %d, %d, %d return to work.\n", eArr[0]->GetElvesID() + 1, eArr[1]->GetElvesID() + 1, eArr[2]->GetElvesID() + 1);
		PrintText(WriteBuffer);
		WaitElves->Signal();
	}
	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::ReindeerBack (Monitor Procedure)             */
/*    This is a monitor procedure for when reindeer returns    */
/*    The logic for them is laid out in this function.         */
/* PARAMETER USAGE :                                           */
/*    reindeer - Pass along an reindeer object and get         */
/*    current info.                                            */
/* FUNCTION CALLED:                                            */
/*    Reindeer::ThreadFunc()                                   */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::ReindeerBack(Reindeer *reindeer) {
	MonitorBegin();
	char WriteBuffer[10000];
	currentNumOfReindeer++;
	if (currentNumOfReindeer < TotalNumberOfReindeer) {
		sprintf(WriteBuffer, "    Reindeer %d returns\n", reindeer->GetReindeerID() + 1);
		PrintText(WriteBuffer);
		WaitDeer->Wait();
	}
	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::ReindeerBack (Monitor Procedure)             */
/*    This is a monitor procedure for when reindeer wait.      */
/*    The logic for them is laid out in this function.         */
/* PARAMETER USAGE :                                           */
/*    reindeer - Pass along an reindeer object and get         */
/*    current info.                                            */
/* FUNCTION CALLED:                                            */
/*    Reindeer::ThreadFunc()                                   */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::WaitOthers(Reindeer *reindeer) {
	MonitorBegin();
	char WriteBuffer[10000];

	if (currentNumOfReindeer == TotalNumberOfReindeer) {
		sprintf(WriteBuffer, "    The last reindeer %d wakes up Santa.\n", reindeer->GetReindeerID() + 1);
		PrintText(WriteBuffer);
		previousReindeer = reindeer;
		Wake->Signal();
	} else {
		WaitDeer->Wait();
	}

	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::WaitSleigh (Monitor Procedure)               */
/*    This is a monitor procedure for when reindeer need the   */
/*    sleigh. The logic for them is laid out in this function. */
/* PARAMETER USAGE :                                           */
/*    reindeer - Pass along an reindeer object and get         */
/*    current info.                                            */
/* FUNCTION CALLED:                                            */
/*    Reindeer::ThreadFunc()                                   */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::WaitSleigh(Reindeer *reindeer) {
	MonitorBegin();
	char WriteBuffer[10000];
	WaitElves->Wait();
	if (reindeer == previousReindeer) {
		sprintf(WriteBuffer, "Santa is preparing sleighs\n");
		PrintText(WriteBuffer);
		CompleteSantaTask->Wait();
	}
	WaitElves->Signal();
	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::FlyOff (Monitor Procedure)                   */
/*    This is a monitor procedure for when reindeer fly in the */
/*    sleigh. The logic for them is laid out in this function. */
/* PARAMETER USAGE :                                           */
/*    reindeer - Pass along an reindeer object and get         */
/*    current info.                                            */
/* FUNCTION CALLED:                                            */
/*    Reindeer::ThreadFunc()                                   */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::FlyOff(Reindeer *reindeer) {
	MonitorBegin();
	if (reindeer == previousReindeer) {
		currentNumOfReindeer = 0;
		WaitDeer->Signal();
		CompleteSantaTask->Signal();
	}
	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::Sleep (Monitor Procedure)                    */
/*    This is a monitor procedure for when Santa's sleeps.     */
/*    The logic for them is laid out in this function.         */
/* PARAMETER USAGE :                                           */
/*    N/A               		                               */
/* FUNCTION CALLED:                                            */
/*    Santa::ThreadFunc()                                      */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::Sleep() {
	int event;
	MonitorBegin();
	Wake->Wait();
	if (currentNumOfReindeer == TotalNumberOfReindeer) {
		event = REINDEEREV;
	} else if (currentNumOfElves == 3 && currentNumOfElves >= 3) {
		event = ELVEDEV;
	}
	MonitorEnd();
	return event;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::ElvesRequest (Monitor Procedure)             */
/*    This is a monitor procedure for when Elves's get answered*/
/*    The logic for them is laid out in this function.         */
/* PARAMETER USAGE :                                           */
/*    N/A               		                               */
/* FUNCTION CALLED:                                            */
/*    Elves::ThreadFunc()                                      */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::ElvesRequest() {
	MonitorBegin();
	char WriteBuffer[10000];
	sprintf(WriteBuffer, "Santa answers the question posted by elves %d, %d, %d.\n", eArr[0]->GetElvesID() + 1, eArr[1]->GetElvesID() + 1, eArr[2]->GetElvesID() + 1);
	PrintText(WriteBuffer);
	used = 0;
	CompleteSantaTask->Signal();
	MonitorEnd();
	return 1;
}
/* ----------------------------------------------------------- */
/* FUNCTION: Mon::ReindeerRequest (Monitor Procedure)          */
/*    This is a monitor procedure for when Reindeer's get      */
/*    need to use santa to fly                                 */
/*    The logic for them is laid out in this function.         */
/* PARAMETER USAGE :                                           */
/*    int numOfTimes = number of times santa flies             */
/* FUNCTION CALLED:                                            */
/*    ReindeerRequest::ThreadFunc()                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::ReindeerRequest(int numOfTimes) {
	MonitorBegin();
	char WriteBuffer[10000];
	used = 1;
	
	sprintf(WriteBuffer, "The team flies off (%d)!\n", numOfTimes);
	PrintText(WriteBuffer);
	
	used = 0;
	CompleteSantaTask->Signal();
	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::RetSanta (Monitor Procedure)                 */
/*    This is a monitor procedure for when santa is at his     */
/*    point in which he needs to retire. This will force him to*/
/*    retire.                                             	   */
/* PARAMETER USAGE :                                           */
/*    N/A               		                               */
/* FUNCTION CALLED:                                            */
/*    Santa::ThreadFunc()                                      */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::RetSanta() {
	MonitorBegin();
	retiredSanta = 1;
	MonitorEnd();
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::RetSanta (Monitor Procedure)                 */
/*    This is a monitor procedure for check to see if santa    */
/*    is retired or not.                                       */
/* PARAMETER USAGE :                                           */
/*    N/A               		                               */
/* FUNCTION CALLED:                                            */
/*    Santa::ThreadFunc()                                      */
/*    Reindeer::ThreadFunc()                                   */
/*    Elves::ThreadFunc()                                      */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::Retired() {
	return retiredSanta;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Mon::PrintText (Monitor Procedure)                */
/*    This is a monitor procedure for check to aid in printing *.
/*    text                                                     */
/* PARAMETER USAGE :                                           */
/*    N/A               		                               */
/* FUNCTION CALLED:                                            */
/*    Santa::ThreadFunc()                                      */
/*    Reindeer::ThreadFunc()                                   */
/*    Elves::ThreadFunc()                                      */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Mon::PrintText(char * buf) {
	char writeBuffer[10000];
	sprintf(writeBuffer, buf);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	return 1;
}

//--------------------------------------------------------------------//

/* ----------------------------------------------------------- */
/* FUNCTION: Elves::Elves                                      */
/*    This is the constructor for the Reindeer Thread          */
/* PARAMETER USAGE :                                           */
/*    int ThreadID - To keep track of the Elves i'th num.      */
/*	  Mon mon - A Pointer to the Monitor Mon being used.       */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
Elves::Elves(int ThreadID, Mon *mon) : mon(mon) {
	ElvesID = ThreadID;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Elves::GetElvesID()                               */
/*    This returns the ThreadID of the current Elves.          */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Elves::GetElvesID() {
	return ElvesID;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Elves                                             */
/*    This is the logic behind the Elves Thread being used.    */
/* PARAMETER USAGE :                                           */
/*    int ThreadID - To keep track of the Elves i'th num.      */
/*	  Mon mon - A Pointer to the Monitor Mon being used.       */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
void Elves::ThreadFunc() {
	Thread::ThreadFunc();
	char WriteBuffer[10000];
	
	sprintf(WriteBuffer, "         Elf %d starts.\n", ElvesID + 1);
	mon->PrintText(WriteBuffer);
	while (mon->Retired() == 0) {
		Delay();
		mon->AskQuestion(this);
		Delay();
	}

	Exit();
}

//--------------------------------------------------------------------//

/* ----------------------------------------------------------- */
/* FUNCTION: Reindeer::Reindeer                                */
/*    This is the constructor for the Reindeer Thread          */
/* PARAMETER USAGE :                                           */
/*    int ThreadID - To keep track of the Reindeer i'th num.   */
/*	  Mon mon - A Pointer to the Monitor Mon being used.       */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
Reindeer::Reindeer(int threadId, Mon *mon) : mon(mon) {
	ReindeerID = threadId;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Reindeer::GetReindeerID                           */
/*    This returns the ThreadID of the current Reindeer.       */
/* PARAMETER USAGE :                                           */
/*    N/A                                                      */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
int Reindeer::GetReindeerID() {
	return ReindeerID;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Reindeer::ThreadFunc()                            */
/*    This is the logic behind the Santa Thread being used.    */
/* PARAMETER USAGE :                                           */
/*    int ThreadID - To keep track of the Reindeer i'th num.   */
/*	  Mon mon - A Pointer to the Monitor Mon being used.       */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
void Reindeer::ThreadFunc() {
	Thread::ThreadFunc();
	char WriteBuffer[10000];
	sprintf(WriteBuffer, "    Reindeer %d starts.\n", GetReindeerID() + 1);
	mon->PrintText(WriteBuffer);
	
	while (mon->Retired() == 0){
		Delay();
		mon->ReindeerBack(this);
		mon->WaitOthers(this);
		mon->WaitSleigh(this);
		mon->FlyOff(this);
		Delay();
	}

	Exit();
}

//--------------------------------------------------------------------//

/* ----------------------------------------------------------- */
/* FUNCTION: Santa::Santa                                      */
/*    This is the constructor for the Santa Threads being used */
/* PARAMETER USAGE :                                           */
/*    int t - Total number of toys                             */
/*	  Mon mon - A Pointer to the Monitor Mon being used.       */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
Santa::Santa(int t, Mon *mon) : mon(mon) {
	mymon = mon;
	TotalNumOfToys = t;
}

/* ----------------------------------------------------------- */
/* FUNCTION: Santa                                             */
/*    This is the logic behind the Santa Thread being used.    */
/* PARAMETER USAGE :                                           */
/*    int t - Total number of toys                             */
/*	  Mon mon - A Pointer to the Monitor Mon being used.       */
/* FUNCTION CALLED:                                            */
/*    ThreadMentor                                             */
/* ----------------------------------------------------------- */
void Santa::ThreadFunc() {
	Thread::ThreadFunc();
	
	char WriteBuffer[10000];
	mon->PrintText("Santa thread starts.\n");
	
	int numOfToysMade = 0;
	while (mon->Retired() == 0){ 
		if (numOfToysMade == TotalNumOfToys) {
			mon->RetSanta();
			break;
		}
		Delay();
		int option = mon->Sleep();
		if (option == REINDEEREV) {
			mon->PrintText("Santa is preparing sleighs.\n");
			mon->ReindeerRequest(++numOfToysMade);
		}
		
		if (option == ELVEDEV) {
			mon->PrintText("Santa is helping elves.\n");
			mon->ElvesRequest();
		}
	}
	
	sprintf(WriteBuffer, "After %d  deliveries, Santa retires and is on vacation!\n", t);
	mon->PrintText(WriteBuffer);
	Exit();
}