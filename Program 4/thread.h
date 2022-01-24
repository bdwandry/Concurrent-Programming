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
/* will remove everyone from that room. This files function is */
/* to create a link and shared memory between the threads.     *.
/* As wellas creating a link to threadmentor in the backend.   */
/* ----------------------------------------------------------- */

#include "ThreadClass.h"

//Global variables that share values between Main and thread.cpp class
extern Semaphore ExSem;
extern Semaphore EntSem;
extern Mutex PtrLck;

/* ----------------------------------------------------------- */
/* FUNCTION: StudentsThread (Class Definition)                 */
/*    This is the header file that describes the threadmentor's*/
/*	  class. This class will be shared with all files when	   */
/*	  threads are called for student based threads.            */
/* PARAMETER USAGE :                                           */
/*    int i - used to pass through the iteration index         */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class StudentsThread : public Thread {
    public:
        StudentsThread(int i);
		static int studentsInRoom;
		static int totalProcesses;

    private:
		int i; int studentID;
		int GoToParty(); void ThreadFunc();

};

/* ----------------------------------------------------------- */
/* FUNCTION: LandLordThread (Class Definition)                 */
/*    This is the header file that describes the threadmentor's*/
/*	  class. This class will be shared with all files when	   */
/*	  threads are called for student based threads.            */
/* PARAMETER USAGE :                                           */
/*    int m - used for number of times the landlord checks the */
/*            room                                             */
/*    int n - Keeps track of the rooms capacity                */
/* FUNCTION CALLED :                                           */
/*    thread.cpp                                               */
/*    Threadmentor                                             */ 
/* ----------------------------------------------------------- */
class LandLordThread : public Thread {
    public:
        LandLordThread(int m, int n);
		static int completed;

    private:
		int m; int n; int numOfAptmSearches; int capacity; int iteration;
		int CheckRoom(); void ThreadFunc();
};