/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 11/13/2021                                       */
/* PROGRAM ASSIGNMENT 2                                        */
/* FILE NAME : merge.c                                         */
/* PROGRAM PURPOSE : Thus is an algorithm that will do Qsort   */
/* on an Array that is shared via shared memory by main. It    */
/* will sort data in ascending order. Everything is done       */
/* concurently as well.                                        */ 
/* ----------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

/* ----------------------------------------------------------- */
/* FUNCTION: main                                              */
/*    This function is the main function to this program       */
/*    It will be the starting point for the program as well as */
/*    Spawn all of the extra processes                         */
/* PARAMETER USAGE :                                           */
/*    Uses: argv[1]...argv[5] for getting relavent information */
/*    from Main                                                */
/* FUNCTION CALLED :                                           */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char * argv[]) {
	char writeBuffer[10000];
	int k = atoi(argv[2]);
	int m = atoi(argv[3]);
	int n = atoi(argv[4]);
	//Initialize shared Memory between main and merge
	key_t mainSharedMemKey = atoi(argv[1]);
	int sharedMemoryID = shmget(mainSharedMemKey, sizeof(int) * (k + m + n), 0666);
	
	key_t mergeSharedMemKey = atoi(argv[5]);
	int mergeSharedMemID = shmget(mergeSharedMemKey, sizeof(int) * (m + n), 0666);

	//Setting up Shared Memory Access with main
	int * sharedMemAttach = (int *)shmat(sharedMemoryID, NULL, 0);
	int * mergeSharedMemAttach = (int *)shmat(mergeSharedMemID, NULL, 0);

	//Setting up the n + m process
	pid_t pid[n + m];
	
	//Binary Search
	int counter = 0;
	int searchEl = mergeSharedMemAttach[counter];
	int index = 0;
	int start = 0;
	int end = m + n;
	int middle;
	while (start <= end){
		int middle = start + (end- start )/2;
		if (mergeSharedMemAttach[middle] == searchEl) {
			index = middle;
		}
		if (mergeSharedMemAttach[middle] < searchEl) {
			start = middle + 1;
		}
		else {
			end = middle - 1;
		}
	}
	for (int i = 0; i < (m + n); i++) {
		if (mergeSharedMemAttach[i] > mergeSharedMemAttach[index] && mergeSharedMemAttach[i] < mergeSharedMemAttach[index]) {
			int temp = mergeSharedMemAttach[i];
			mergeSharedMemAttach[i] = mergeSharedMemAttach[index];
			mergeSharedMemAttach[i + 1] = temp;
			break;
		}
	}
	sprintf(writeBuffer, "### PROC(%d): exits\n", getpid()); 
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	for (int i = 0; i < m; i++) {
		if ((pid[i] = fork()) == 0) {
			counter++;
			char * mcommand =  "./merge";
			char * marguments[7];
			char margumentBuffer[10000];
			
			marguments[0] = "./merge";
			
			sprintf(margumentBuffer, "%d", mainSharedMemKey);
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
	
			sprintf(writeBuffer, "### M-PROC(%d): section a[%d..%d] sorted\n    ", getpid(), 0, atoi(argv[4])); 
			write(1, writeBuffer, strlen(writeBuffer));
			memset(writeBuffer, 0, 10000);
			for (int i = 0; i < (n+m); i++) {
				sprintf(writeBuffer + strlen(writeBuffer), "%d ", mergeSharedMemAttach[i]);
			}
			sprintf(writeBuffer + strlen(writeBuffer), "\n");
			write(1, writeBuffer, strlen(writeBuffer));
			memset(writeBuffer, 0, 10000);
			
			marguments[6] = NULL;
			execvp(mcommand, marguments);
			exit(0);
		}
		//printf("%d ",sharedMemAttach[i + k + m]); 
		if ((pid[i] = fork()) == 0) {
			char * mcommand =  "./merge";
			char * marguments[7];
			char margumentBuffer[10000];
			
			marguments[0] = "./merge";
			
			sprintf(margumentBuffer, "%d", mainSharedMemKey);
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
			
			sprintf(writeBuffer, "### M-PROC(%d): section a[%d..%d] sorted\n", getpid(), 0, atoi(argv[4])); 
			write(1, writeBuffer, strlen(writeBuffer));
			memset(writeBuffer, 0, 10000);
			sprintf(writeBuffer, "    ");
			for (int i = 0; i < (n+m); i++) {
				sprintf(writeBuffer + strlen(writeBuffer), "%d ", mergeSharedMemAttach[i]);
			}
			sprintf(writeBuffer + strlen(writeBuffer), "\n");
			write(1, writeBuffer, strlen(writeBuffer));
			memset(writeBuffer, 0, 10000);
	
			marguments[6] = NULL;
			execvp(mcommand, marguments);
			exit(0);
		}
		
	}
	
	//Waiting
	int status;
	for (int i = 0; i < (atoi(argv[3])+atoi(argv[4])); i++) {
		wait(&status);
	}	
	//Detaching Information
	shmdt(mergeSharedMemAttach);
	shmctl(mergeSharedMemID, IPC_RMID, NULL);
	shmdt(sharedMemAttach);
	sprintf(writeBuffer, "***Main: merge shared memory successfully detached\n");
	// write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	shmctl(sharedMemoryID, IPC_RMID, NULL);
	sprintf(writeBuffer, "***Main: merge shared memory successfully removed\n");
	// write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	return 1;
}