/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 10/13/2021                                       */
/* PROGRAM ASSIGNMENT 2                                        */
/* FILE NAME : main.c                                          */
/* PROGRAM PURPOSE : This is a main method that will interact  */
/*  with several different types of programs. It will take in  */
/*  an input file full of randoized data. And it will be passed*/
/*  through their respected algorithms                         */
/* ----------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
/* ----------------------------------------------------------- */
/* FUNCTION: main                                              */
/*    This function is the main function to this program       */
/*    It will be the starting point for the program as well as */
/*    Spawn all of the extra processes                         */
/* PARAMETER USAGE :                                           */
/*    Reads in STDIN usage of a data file with 6 lines         */
/*    of inputs                                                */
/* FUNCTION CALLED :                                           */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main (void) {
	//Qsort Initialization
	int k = 0;
	int * aPointer;
	
	//Merge Initialization for x[]
	int m = 0;
	int * xPointer;
	
	//Merge Initialization for y[]
	int n = 0;
	int * yPointer;
	
	//Reading in the input file from STDIN and Parsing file
    char buffer[100000];
	int counter = 1;
	int globalSize = 0;
	while (fgets(buffer + strlen(buffer), 10000, stdin) != NULL) {
		if (counter % 2 == 1) {
			if (counter == 1) {
				k = atoi(buffer);
				globalSize = k;
			}
			if (counter == 3) {
				m = atoi(buffer);
				globalSize = m;
			}
			
			if (counter == 5) {
				n = atoi(buffer);
				globalSize = n;
			}	
			memset(buffer, 0, 100000);
		}
		
		if (counter % 2 == 0) {
			char tempBuffer[100000];
			sprintf(tempBuffer, "%s", buffer);
			int tempNumArr[globalSize];
			
			int tempCounter = 0;
			char * token = strtok(tempBuffer, " ");
			while( token != NULL ) {
				tempNumArr[tempCounter++] = atoi(token);
				token = strtok(NULL, " ");
			}
			
			if (counter == 2) {
				aPointer = (int *)malloc(sizeof(int)*k);
				for (int i = 0; i < k; i++) {
					aPointer[i] = tempNumArr[i];
				}
			}
			if (counter == 4) {
				xPointer = (int *)malloc(sizeof(int) * m);
				for (int i = 0; i < m; i++) {
					xPointer[i] = tempNumArr[i];
				}
			}
			
			if (counter == 6) {
				yPointer =( int *)malloc(sizeof(int) * n);
				for (int i = 0; i < n; i++) {
					yPointer[i] = tempNumArr[i];
				}
			}
			memset(buffer, 0, 100000);
		}
		
		counter++;
	}
	
	//loading pointerData into arrays
	int a[k];
	for (int i = 0; i < k; i++) {
		a[i] = aPointer[i];
	}
	
	int x[m];
	for (int i = 0; i < m; i++) {
		x[i] = xPointer[i];
	}
	
	int y[n];
	for (int i = 0; i < n; i++) {
		y[i] = yPointer[i];
	}
	
	//Initial Printouts
	char writeBuffer[10000];
	sprintf(writeBuffer, "Quicksort and Binary Merge with Multiple Processes:\n\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//Setting up Shared Memory Key and printing it
	key_t sharedMemKey = ftok("./", 'x');
	sprintf(writeBuffer, "*** MAIN: shared memory key = %d\n", sharedMemKey);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//Setting up and printing out - shared memory allocation
	int totalKNMCount = k + n + m;
	int sharedMemID = shmget(sharedMemKey, sizeof(int) * totalKNMCount, (IPC_CREAT | 0666));
	sprintf(writeBuffer, "*** MAIN: shared memory created\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//Setting up Shared Memory Access and printing it out
	int * sharedMemAttach = (int *)shmat(sharedMemID, NULL, 0);
	sprintf(writeBuffer, "*** MAIN: shared memory attached and is ready to use\n\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//Creating Shared memory between the two arrays
	key_t mergeSharedMemKey = ftok("./", 'z');
	int mergeSharedMemID = shmget(mergeSharedMemKey, sizeof(int) * (m + n), (IPC_CREAT | 0666));
	int * mergeSharedMemAttach = (int *)shmat(mergeSharedMemID, NULL, 0);

	//Printing out infomation about k, m, n, a[], x[], y[]
	//K and A[]
	sprintf(writeBuffer, "Input array for qsort has %d elements:\n", k);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	sprintf(writeBuffer, "    ");
	for (int i = 0; i < k; i++) {
		sprintf(writeBuffer + strlen(writeBuffer), "%d ", a[i]);
	}
	sprintf(writeBuffer + strlen(writeBuffer), "\n\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//m and x[]
	sprintf(writeBuffer, "Input array x[] for merge has %d elements:\n", m);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	sprintf(writeBuffer, "    ");
	for (int i = 0; i < m; i++) {
		sprintf(writeBuffer + strlen(writeBuffer), "%d ", x[i]);
	}
	sprintf(writeBuffer + strlen(writeBuffer), "\n\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//n and y[]
	sprintf(writeBuffer, "Input array y[] for merge has %d elements:\n", n);
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	sprintf(writeBuffer, "    ");
	for (int i = 0; i < n; i++) {
		sprintf(writeBuffer + strlen(writeBuffer), "%d ", y[i]);
	}
	sprintf(writeBuffer + strlen(writeBuffer), "\n\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//Loads all of the data into the shared memory
	//Load k and a[]
	for (int i = 0; i < k; i++) {
		sharedMemAttach[i] = a[i];
	}
	
	//Load m and x[]
	for (int i = 0; i < m; i++) {
		sharedMemAttach[i + k] = x[i];
	}

	//Load n and y[]
	for (int i = 0; i < n; i++) {
		sharedMemAttach[i + k + m] = y[i];
	}
	
	for (int i = 0; i < n; i++) {
		mergeSharedMemAttach[i] = sharedMemAttach[i + k];
	}
	
	//Creating the Qsort Command
	char * command =  "./qsort";
	char * arguments[6];
	char argumentBuffer[10000];
	
	arguments[0] = "./qsort";
	
	sprintf(argumentBuffer, "%d", sharedMemKey);
	arguments[1] = strdup(argumentBuffer);
	memset(argumentBuffer, 0, 10000);

	arguments[2] = "0";
	
	sprintf(argumentBuffer, "%d", k - 1);
	arguments[3] = strdup(argumentBuffer);
	memset(argumentBuffer, 0, 10000);

	sprintf(argumentBuffer, "%d", totalKNMCount);
	arguments[4] = strdup(argumentBuffer);
	memset(argumentBuffer, 0, 10000);
	
	arguments[5] = NULL;
	
	//Setting up Merge for x[] and y[]
	char * mcommand =  "./merge";
	char * marguments[7];
	char margumentBuffer[10000];
	
	marguments[0] = "./merge";
	
	sprintf(margumentBuffer, "%d", sharedMemKey);
	marguments[1] = strdup(margumentBuffer);
	memset(margumentBuffer, 0, 10000);

	sprintf(margumentBuffer, "%d", k);
	marguments[2] = strdup(margumentBuffer);
	memset(margumentBuffer, 0, 10000);
	
	sprintf(margumentBuffer, "%d", m);
	marguments[3] = strdup(margumentBuffer);
	memset(margumentBuffer, 0, 10000);
	
	sprintf(margumentBuffer, "%d", n);
	marguments[4] = strdup(margumentBuffer);
	memset(margumentBuffer, 0, 10000);
	
	sprintf(margumentBuffer, "%d", mergeSharedMemKey);
	marguments[5] = strdup(margumentBuffer);
	memset(margumentBuffer, 0, 10000);
	
	marguments[6] = NULL;
	
	//Writing out that children process are about to start
	sprintf(writeBuffer, "*** MAIN: about to spawn the process for qsort\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	//Starting the 2 different process
	pid_t pid[2];
	if ((pid[0] = fork()) == 0) {
		execvp(command, arguments);
		exit(0);
	}
	
	sprintf(writeBuffer, "*** MAIN: about to spawn the process for merge\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	if ((pid[1] = fork()) == 0) {
		execvp(mcommand, marguments);
		exit(0);
	}
	
	int status;
	for (int i = 0; i < 2; i++) {
		wait(&status);
	}
	
	//Writes the output of the Qsort Array from shared memory
	sprintf(writeBuffer, "*** MAIN: sorted array by qsort:\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	sprintf(writeBuffer, "    ");
	for (int i = 0; i < k; i++) {
		sprintf(writeBuffer + strlen(writeBuffer), "%d ", sharedMemAttach[i]);
	}
	sprintf(writeBuffer + strlen(writeBuffer), "\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	//Copying mergeSharedMemAttach to sharedMemAttach
	for (int i = 0; i < (n + m); i++) {
		sharedMemAttach[i + k] = mergeSharedMemAttach[i];
	}
 	
	sprintf(writeBuffer, "*** MAIN: merged array:\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	sprintf(writeBuffer, "    ");
	for (int i = 0; i < m + n; i++) {
		sprintf(writeBuffer + strlen(writeBuffer), "%d ", sharedMemAttach[i + k]);
	}
	sprintf(writeBuffer + strlen(writeBuffer), "\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
    
	//Writes the output for detaching
	shmdt(sharedMemAttach);
	shmdt(mergeSharedMemAttach);
	sprintf(writeBuffer, "***MAIN: shared memory successfully detached\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	shmctl(sharedMemID, IPC_RMID, NULL);
	shmctl(mergeSharedMemID, IPC_RMID, NULL);
	sprintf(writeBuffer, "***MAIN: shared memory successfully removed\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
}