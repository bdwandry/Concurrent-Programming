/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 11/3/2021                                        */
/* PROGRAM ASSIGNMENT 3                                        */
/* FILE NAME : thread-main.cpp                                 */
/* PROGRAM PURPOSE : To conduct an sorting algorithm based on  */
/* even-odd paradigm comcurrently. This files function is to   */
/* deal with reading a file using CPP calls and functions and  */
/* setup/iterate through even and odd sort. Throughout the     */
/* iteration process, it will create the compute threads.      */
/* ----------------------------------------------------------- */
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

//Global variables that are created to be used by all different files linked together. 
int arrSize; 
int * arrData;
char mainBuff[10000];

/* ----------------------------------------------------------- */
/* FUNCTION: printArray                                        */
/*    This function is used for no more than printing the array*/
/*	  data at different points in time. Its mains purpose is to*/ 
/*	  consolitdate code into a single function.                */
/* PARAMETER USAGE :                                           */
/*    option = Prints out different statement depending on	   */
/*	  the specified value passed through.					   */
/*	  value = A specific value used for the 3rd option of	   */
/*	  of printing.											   */
/* FUNCTION CALLED :                                           */
/*    Main                                                     */
/* ----------------------------------------------------------- */
int printArray(int option, int val) {
	
	//Prints out beginning of the programs information
	if (option == 1) {
		write(1, "Concurrent Even-Odd Sort\n\n", 26);
		sprintf(mainBuff, "Number of input data = %d\n", arrSize);
		write(1, mainBuff, strlen(mainBuff));		
		memset(mainBuff, 0, 10000);
		write(1, "Input Array:\n   ", 16);
	}
	
	//Prints out end of the programs information
	if (option == 2) {
		//write(1, "Final result after iteration j:\n   ", 35);
		sprintf(mainBuff, "Final result after iteration %d:\n   ", val);
		write(1, mainBuff, strlen(mainBuff));		
		memset(mainBuff, 0, 10000);
	}
	
	//Prints out iteration i of the programs information
	if (option == 3) {
		sprintf(mainBuff, "Result after iteration %d:\n   ", val);
		write(1, mainBuff, strlen(mainBuff));		
		memset(mainBuff, 0, 10000);
	}
	
	
	//Tries to do a basic format print of the array data based on the option passed through.
	for (int i = 0; i < arrSize; i++) {
		if ((i % 20 == 0) && (i != 0)) {
			sprintf(mainBuff + strlen(mainBuff), "\n   ");
		}
		
		if ((option == 1) || (option == 2)) {
			if ((arrData[i] < 10) && (arrSize > 20)) {
				sprintf(mainBuff + strlen(mainBuff), "%d    ", arrData[i]);
			} else {
				sprintf(mainBuff + strlen(mainBuff), "%d   ", arrData[i]);
			}
		}
		
		if ((option == 3)) {
			if ((arrData[i] < 10) && (arrSize > 20)) {
				sprintf(mainBuff + strlen(mainBuff), "%d  ", arrData[i]);
			} else {
				sprintf(mainBuff + strlen(mainBuff), "%d ", arrData[i]);
			}
			
		}
	}
	write(1, mainBuff, strlen(mainBuff));		
	memset(mainBuff, 0, 10000);
	write(1, "\n",1);
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: startThreadmentorEven                             */
/*    The porpose of this program is to spin up all the 	   */    
/*	  threads and iterate through all of the even indices of   */
/*	  the array data.   									   */
/* PARAMETER USAGE :                                           */
/*    Reads data from the global array data and array size     */
/* FUNCTION CALLED :                                           */
/*    Main                                                     */
/* ----------------------------------------------------------- */
int startThreadmentorEven() {
	EvenOddThread *evenoddthread[arrSize];
	//_________________________________________________________
	//Even iteration 
	write(1, "    Even Pass:\n", 15);
	
	//creates n/2 number of threads
	for (int j = 1; j < arrSize; j += 2) {
		evenoddthread[j] = new EvenOddThread(j);
		evenoddthread[j]->Begin();
	}
	
	//waits for n/2 number of threads
	for (int j = 1; j < arrSize; j += 2) {
		evenoddthread[j]->Join();
	}
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: startThreadmentorOdd                              */
/*    The porpose of this program is to spin up all the 	   */    
/*	  threads and iterate through all of the odd indices of    */
/*	  the array data.   									   */
/* PARAMETER USAGE :                                           */
/*    Reads data from the global array data and array size     */
/* FUNCTION CALLED :                                           */
/*    Main                                                     */
/* ----------------------------------------------------------- */
int startThreadmentorOdd() {
	EvenOddThread *evenoddthread[arrSize];
	// _________________________________________________________
	// Odd iteration
	write(1, "    Odd Pass:\n", 14);
	//creates n/2 number of threads
	for (int j = 2; j < arrSize; j += 2) {
		evenoddthread[j] = new EvenOddThread(j);
		evenoddthread[j]->Begin();
	}
	
	//waits for n/2 number of threads
	for (int j = 2; j < arrSize; j += 2) {
		evenoddthread[j]->Join();
	}
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: main                                              */
/*    Checks to see if all array elements are in order using   */
/*    ascending order.                         				   */
/* PARAMETER USAGE :                                           */
/*    Global ArrData: Searching through the array              */
/*    Global ArrSize: Knowing how many elements there are      */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
int CheckIfNoSwaps() {
	for (int i = 0; i < arrSize-1; i++) {
		if (arrData[i] > arrData[i+1]) {
			return 1;
		}
	}
	return 0;
}

/* ----------------------------------------------------------- */
/* FUNCTION: main                                              */
/*    This function is the main function to this program       */
/*    It will start out by reading in the inputfile passed 	   */
/*	  through STDIN and starts even/odd sort process iterator  */
/* PARAMETER USAGE :                                           */
/*    Reads in STDIN usage of a data file with 2 lines         */
/*    of inputs                                                */
/* FUNCTION CALLED :                                           */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	//Gets the Array Size from Stdin
	cin >> arrSize;
	
	//Gets the array elements from
	//int arrData[arrSize];
	arrData = (int * )malloc(sizeof(int) * arrSize);
	for (int i = 0; i < arrSize; i++) {
		int tempVar;
		cin >> tempVar;
		arrData[i] = tempVar;
	}
	
	//Main Printing out First Information
	printArray(1, 0);
	
	//Starting threadmentor and the iteration
	int swapped = 1;
	int i = 0;
	while (swapped == 1) {
		//Checks to see if all array elements are in order using ascending order.
		swapped = CheckIfNoSwaps();
		if (swapped == 0) {
			break;
		}
		
		//Print out
		sprintf(mainBuff, "Iteration %d:\n", i);
		write(1, mainBuff, strlen(mainBuff));		
		memset(mainBuff, 0, 10000);
		
		//Starts the function to create/sort even elements
		startThreadmentorEven();
		
		//Starts the function to create/sort odd elements
		startThreadmentorOdd();
		
		//Iterates I to keep track of how many swaps it takes to sort.
		i++;
		
		//Prints out the array information
		printArray(3, i);
	}
	
	printArray(2, i);
	return 1;
}