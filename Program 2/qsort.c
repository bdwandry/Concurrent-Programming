/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 11/13/2021                                       */
/* PROGRAM ASSIGNMENT 2                                        */
/* FILE NAME : qsort.c                                         */
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

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int partition(int * arr, int beggining, int end) {
	int pivot = arr[end];
	int i = (beggining - 1);
	for (int j = beggining; j < end; j++) {
		if (arr[j] <= pivot) {
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[end]);
	char writeBuffer[10000];
	sprintf(writeBuffer, "   ### Q-PROC(%d): pivot element is a[%d]\n", getpid(), i + 1); 
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	return (i + 1);
}

void quickSort(int * arr, int beggining, int end) {
	if (beggining < end) {
		char writeBufferLeft[10000];
		int midpoint = partition(arr, beggining, end);
		sprintf(writeBufferLeft, "   ### Q-PROC(%d): entering with a[%d..%d]\n", getpid(), beggining, midpoint); 
		write(1, writeBufferLeft, strlen(writeBufferLeft));
		memset(writeBufferLeft, 0, 10000);
		sprintf(writeBufferLeft, "       ");
		for (int i = beggining; i < midpoint; i++) {
			sprintf(writeBufferLeft + strlen(writeBufferLeft), "%d ", arr[i]);
		}
		sprintf(writeBufferLeft + strlen(writeBufferLeft), "\n");
		write(1, writeBufferLeft, strlen(writeBufferLeft));
		memset(writeBufferLeft, 0, 10000);
		
		quickSort(arr, beggining, midpoint - 1);
		quickSort(arr, midpoint + 1, end);
	}
}

int main(int argc, char * argv[]) {
	//Initialize shared Memory between main and qsort
	key_t mainSharedMemKey = atoi(argv[1]);
	int sharedMemoryID = shmget(mainSharedMemKey, sizeof(int) * atoi(argv[4]), 0666);
	
	//Setting up Shared Memory Access
	int * sharedMemAttach = (int *)shmat(sharedMemoryID, NULL, 0);
	
	//Setting up some inititial buffers
	char writeBuffer[10000];
	
	//Printing out the start of Qsort Process
	sprintf(writeBuffer, "   ### Q-PROC(%d): entering with a[%d..%d]\n", getpid(), atoi(argv[2]), atoi(argv[3])); 
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	int midpoint = partition(sharedMemAttach, atoi(argv[2]), atoi(argv[3]));
	pid_t pid[2];
	
	if ((pid[0] = fork()) == 0) {
		quickSort(sharedMemAttach ,atoi(argv[2]), midpoint);
		sprintf(writeBuffer, "   ### Q-PROC(%d): section a[%d..%d] sorted\n", getpid(), atoi(argv[2]), midpoint); 
		write(1, writeBuffer, strlen(writeBuffer));
		memset(writeBuffer, 0, 10000);
		sprintf(writeBuffer, "       ");
		for (int i = atoi(argv[2]); i < midpoint; i++) {
			sprintf(writeBuffer + strlen(writeBuffer), "%d ", sharedMemAttach[i]);
		}
		sprintf(writeBuffer + strlen(writeBuffer), "\n");
		write(1, writeBuffer, strlen(writeBuffer));
		memset(writeBuffer, 0, 10000);
		
		sprintf(writeBuffer, "   ### Q-PROC(%d): exits\n", getpid()); 
		write(1, writeBuffer, strlen(writeBuffer));
		memset(writeBuffer, 0, 10000);
		exit(0);
	}
	
	if ((pid[1] = fork()) == 0) {
		quickSort(sharedMemAttach ,midpoint, atoi(argv[3]));
		sprintf(writeBuffer, "   ### Q-PROC(%d): section a[%d..%d] sorted\n", getpid(), midpoint, atoi(argv[3])); 
		write(1, writeBuffer, strlen(writeBuffer));
		memset(writeBuffer, 0, 10000);
		sprintf(writeBuffer, "       ");
		for (int i = midpoint; i < atoi(argv[3]); i++) {
			sprintf(writeBuffer + strlen(writeBuffer), "%d ", sharedMemAttach[i]);
		}
		sprintf(writeBuffer + strlen(writeBuffer), "\n");
		write(1, writeBuffer, strlen(writeBuffer));
		memset(writeBuffer, 0, 10000);
		
		sprintf(writeBuffer, "   ### Q-PROC(%d): exits\n", getpid()); 
		write(1, writeBuffer, strlen(writeBuffer));
		memset(writeBuffer, 0, 10000);
		exit(0);
	}
	
	int status;
	for (int i = 0; i < 1; i++) {
		wait(&status);
	}
	
	
 	shmdt(sharedMemAttach);
	sprintf(writeBuffer, "***Main: qsort shared memory successfully detached\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	
	shmctl(sharedMemoryID, IPC_RMID, NULL);
	sprintf(writeBuffer, "***Main: qsort shared memory successfully removed\n");
	write(1, writeBuffer, strlen(writeBuffer));
	memset(writeBuffer, 0, 10000);
	return 1;
}