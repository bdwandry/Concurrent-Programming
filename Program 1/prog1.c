/* ----------------------------------------------------------- */
/* NAME : Bryan Wandrych                     User ID: bdwandry */
/* DUE DATE : 09/28/2021                                       */
/* PROGRAM ASSIGNMENT 1                                        */
/* FILE NAME : prog1.c                                         */
/* PROGRAM PURPOSE : This programs main purpose is to run      */
/* is to concurrently run multiple different processes at the  */
/* at the same time. These processes are not using shared      */
/* memory and interleaving is expected.                        */
/* ----------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//--------------------------------------------------------------------------------------------------//
//This section is dedicated torwards calculating Fibbonacci sequences.
/* ----------------------------------------------------------- */
/* FUNCTION: Fibbonacci                                        */
/*    To calculate a recurisive number up to N integers        */
/*    Using Fibbonacci sequences.                              */
/* PARAMETER USAGE :                                           */
/*    an integer N for being used in the recursive algorithm   */
/* FUNCTION CALLED :                                           */
/*    StartFibbonacciProcess                                   */
/* ----------------------------------------------------------- */
long Fibbonacci(long n) {
   if(n <= 1){
      return n;
   } else {
      return (Fibbonacci(n - 1) + Fibbonacci(n - 2));
   }
}

/* ----------------------------------------------------------- */
/* FUNCTION: StartFibbonacciProcess                            */
/*    Helper function to be called by a process created        */
/*    This function is also used for printing to stout         */
/* PARAMETER USAGE :                                           */
/*    an integer N for being used in the recursive algorithm   */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
int StartFibbonacciProcess(long n) {
	char * FibbonacciBuffer = malloc(10000);
	write(1, "   Fibonacci Process Started\n", 29);
	
	sprintf(FibbonacciBuffer, "   Input Number %ld\n",n);
	write(1, FibbonacciBuffer, strlen(FibbonacciBuffer));		
	memset(FibbonacciBuffer, 0, 10000);
	
	sprintf(FibbonacciBuffer, "   Fibonacci Number f(%d) is %ld\n", n, Fibbonacci(n));
	write(1, FibbonacciBuffer, strlen(FibbonacciBuffer));		
	memset(FibbonacciBuffer, 0, 10000);
	
	write(1, "   Fibonacci Process Exits\n", 27);
}

//--------------------------------------------------------------------------------------------------//
//This is calculating the Buffon's Needle Problem
/* ----------------------------------------------------------- */
/* FUNCTION: BuffonsNeedle                                     */
/*    To calculate if a needle will go out of bounds in an     */
/*    in an infinite sqare sequence                            */
/* PARAMETER USAGE :                                           */
/*    an integer r to determine how much a loop runs (iterates)*/
/* FUNCTION CALLED :                                           */
/*    StartBuffonNeedleProcess                                 */
/* ----------------------------------------------------------- */
float BuffonsNeedle (long r) {
	float calculatedTotal = 0.0;
	float L = 1.0;
	float G = 1.0;
	float t = 0.0;

	srand((unsigned) time(NULL));
	for (long i = 0; i < r; i++) {
		float a = (float)rand()/((float)RAND_MAX/(2.0*acos(-1.0)));
		float d = (float)rand()/((float)RAND_MAX/1);
		calculatedTotal = d + L * sin(a);
		if ((calculatedTotal < 0) || (calculatedTotal > G)) {
			t++;
		}			
	}

	return (t/(float)r);
}

/* ----------------------------------------------------------- */
/* FUNCTION: StartBuffonNeedleProcess                          */
/*    Helper function to be called by a process created        */
/*    This function is also used for printing to stout         */
/* PARAMETER USAGE :                                           */
/*    an integer r to determine how much a loop runs (iterates)*/
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
int StartBuffonNeedleProcess(long r) {
	char * BuffonsNeedleBuffer = malloc(10000);
	write(1, "      Buffon's Needle Process Started\n", 38);
	
	sprintf(BuffonsNeedleBuffer, "      Input Number %ld\n", r);
	write(1, BuffonsNeedleBuffer, strlen(BuffonsNeedleBuffer));		
	memset(BuffonsNeedleBuffer, 0, 10000);
	
	sprintf(BuffonsNeedleBuffer, "      Estimated Probability is %f\n",BuffonsNeedle(r));
	write(1, BuffonsNeedleBuffer, strlen(BuffonsNeedleBuffer));		
	memset(BuffonsNeedleBuffer, 0, 10000);
	
	write(1, "      Buffon's Needle Process Exits\n", 36);
}	

//--------------------------------------------------------------------------------------------------//
//This is calculating area of ellipse
/* ----------------------------------------------------------- */
/* FUNCTION: AreaOfEllipse                                     */
/*    To calculate the Area of an Ellipse.                     */
/*    To also watch and see how this calculate, over enough    */
/*    iterations gets closer to Pi*ab                          */
/* PARAMETER USAGE :                                           */
/*    an integer a, to determine a's value in the equation     */
/*    an integer b, to determine b's value in the equation     */
/*    an integer s,to determine how much a loop runs (iterates)*/
/* FUNCTION CALLED :                                           */
/*    StartAreaOfEllipseProcess                                */
/* ----------------------------------------------------------- */
float AreaOfEllipse (long a, long b, long s) {
	char * AreaOfEllipseBuffer = malloc(10000);
	float calculation;
	float t = 0.0;
	srand((unsigned) time(NULL));
	for (long i = 0; i < s; i++) {
		float x = (float)rand()/((float)RAND_MAX/(float)a);
		float y = (float)rand()/((float)RAND_MAX/(float)b);
		calculation = (pow(x,2)/pow((float)a,2)) + (pow(y,2)/pow((float)b,2));
		if (calculation <= 1.0) {
			t++;
		}
	}
	
	sprintf(AreaOfEllipseBuffer, "         Total Hits %ld\n", (int)t);
	write(1, AreaOfEllipseBuffer, strlen(AreaOfEllipseBuffer));		
	memset(AreaOfEllipseBuffer, 0, 10000);
	
	sprintf(AreaOfEllipseBuffer, "         Estimated Area is %f\n", ((t/s) * (float) a * (float) b) * 4);
	write(1, AreaOfEllipseBuffer, strlen(AreaOfEllipseBuffer));		
	memset(AreaOfEllipseBuffer, 0, 10000);
	
	sprintf(AreaOfEllipseBuffer, "         Actual Area is %f\n", (acos(-1.0)*a*b));
	write(1, AreaOfEllipseBuffer, strlen(AreaOfEllipseBuffer));		
	memset(AreaOfEllipseBuffer, 0, 10000);
	
	return ((t/s) * (float) a * (float) b) * 4;
}

/* ----------------------------------------------------------- */
/* FUNCTION: StartFibbonacciProcess                            */
/*    Helper function to be called by a process created        */
/*    This function is also used for printing to stout         */
/* PARAMETER USAGE :                                           */
/*    an integer a, to determine a's value in the equation     */
/*    an integer b, to determine b's value in the equation     */
/*    an integer s,to determine how much a loop runs (iterates)*/
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
int StartAreaOfEllipseProcess(long a, long b, long s) {
	char * AreaOfEllipseBuffer = malloc(10000);
	write(1, "         Ellipse Area Process Started\n", 38);
	
	sprintf(AreaOfEllipseBuffer, "         Total random Number Pairs %ld\n", s);
	write(1, AreaOfEllipseBuffer, strlen(AreaOfEllipseBuffer));		
	memset(AreaOfEllipseBuffer, 0, 10000);
	
	sprintf(AreaOfEllipseBuffer, "         Semi-Major Axis Length %ld\n", a);
	write(1, AreaOfEllipseBuffer, strlen(AreaOfEllipseBuffer));		
	memset(AreaOfEllipseBuffer, 0, 10000);
	
	sprintf(AreaOfEllipseBuffer, "         Semi-Minor Axis Length %ld\n", b);
	write(1, AreaOfEllipseBuffer, strlen(AreaOfEllipseBuffer));		
	memset(AreaOfEllipseBuffer, 0, 10000);
	
	AreaOfEllipse(a, b, s);
	
	write(1, "         Ellipse Area Process Exits\n", 36);
}

//--------------------------------------------------------------------------------------------------//
//This is for the simple pinball game
/* ----------------------------------------------------------- */
/* FUNCTION: PrintArray                                        */
/*    This function is to give a formated printout to Stout    */
/*    Of how many pinballs fell into each "bin"                */
/* PARAMETER USAGE :                                           */
/*    an int [] BinsFilled, this is all balls in bins          */
/*    an integer x, number of bins generated                   */
/*    an integer y,how many balls were dropped into these bings*/
/* FUNCTION CALLED :                                           */
/*    PinballGame                                              */
/* ----------------------------------------------------------- */
 int PrintArray(long BinsFilled [], long x, long y) {
	char * PrintArrayBuffer = malloc(10000);
	float highestPercentage = 0.0;
	long highestPercentageIndex = 0;
	
	for	(long i = 0; i < x; i++) {
		if ( (((float)BinsFilled[i]/(float)y) * 100) > highestPercentage) {
			highestPercentage = (((float)BinsFilled[i]/(float)y) * 100);
			highestPercentageIndex = i;
		}
	}
	
	for (long i = 0; i < x; i++) {
		sprintf(PrintArrayBuffer, "%3d-(%7ld)-(%5.2f%)|", (i + 1), BinsFilled[i], ((float)BinsFilled[i]/(float)y) * 100);
		int NumOfAsterix = round(((((float)BinsFilled[i]/(float)y) * 100)/highestPercentage) * 50);
		for (int j = 0; j < NumOfAsterix; j++) {
			sprintf(PrintArrayBuffer + strlen(PrintArrayBuffer), "*");
		}
		sprintf(PrintArrayBuffer + strlen(PrintArrayBuffer), "\n");
		write(1, PrintArrayBuffer, strlen(PrintArrayBuffer));		
		memset(PrintArrayBuffer, 0, 10000);
	}
} 

/* ----------------------------------------------------------- */
/* FUNCTION: PinballGame                                       */
/*    This is the function that will randomly sort the balls   */
/*    into n number of generated bins                          */
/* PARAMETER USAGE :                                           */
/*    an int [] BinsFilled, this is all balls in bins          */
/*    an integer x, number of bins generated                   */
/*    an integer y,how many balls were dropped into these bings*/
/* FUNCTION CALLED :                                           */
/*    StartPinballGameProcess                                  */
/* ----------------------------------------------------------- */
int PinballGame (long x, long y) {
	long BinsFilled [x];
	for (int i = 0; i < x; i++) {
		BinsFilled[i] = 0;
	}
	
	srand((unsigned) time(NULL));
	for (long i = 0; i < y; i++) {
		long ballDirection = 0;
		
		for (long j = 0; j < x-1; j++) {
			float random = (float)rand()/((float)RAND_MAX/1);
			if (random >= .5) {
				ballDirection++;
			}
		}
		BinsFilled[ballDirection]++;
	}
	PrintArray(BinsFilled, x, y);
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION: StartPinballGameProcess                           */
/*    Helper function to be called by a process created        */
/*    This function is also used for printing to stout         */
/* PARAMETER USAGE :                                           */
/*    an int [] BinsFilled, this is all balls in bins          */
/*    an integer x, number of bins generated                   */
/*    an integer y,how many balls were dropped into these bings*/
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
int StartPinballGameProcess(long x, long y) {
	char * PinballGameBuffer = malloc(10000);
	write(1, "Simple Pinball Process Started\n", 31);
	
	sprintf(PinballGameBuffer, "Number of Bins %ld\n", x);
	write(1, PinballGameBuffer, strlen(PinballGameBuffer));		
	memset(PinballGameBuffer, 0, 10000);
	
	sprintf(PinballGameBuffer, "Number of Ball Droppings %ld\n", y);
	write(1, PinballGameBuffer, strlen(PinballGameBuffer));		
	memset(PinballGameBuffer, 0, 10000);
	
	PinballGame(x,y);
	
	write(1, "Simple Pinball Process Exits\n", 29);
}


//--------------------------------------------------------------------------------------------------//
//This is the main function
/* ----------------------------------------------------------- */
/* FUNCTION: main                                              */
/*    This function is the main function to this program       */
/*    It will be the starting point for the program as well as */
/*    Spawn all of the extra processes                         */
/* PARAMETER USAGE :                                           */
/*    argc and argv to pass various arguments through the      */
/*    command line once the program is compiled                */
/* FUNCTION CALLED :                                           */
/*    N/A                                                      */
/* ----------------------------------------------------------- */
int main(int argc, char *argv[]) {
	if (argc != 8) {
		printf("./prog1 n r a b s x y\n");
	} else {
		//Main Process Starting
 		char * MainProcessBuffer = malloc(10000);
		write(1, "Main Process Started\n", 21);
	
		sprintf(MainProcessBuffer, "Fibonacci Input            = %ld\n", atol(argv[1]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		sprintf(MainProcessBuffer, "Buffon's Needle Iterations = %ld\n", atol(argv[2]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		sprintf(MainProcessBuffer, "Total random Number Pairs  = %ld\n", atol(argv[3]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		sprintf(MainProcessBuffer, "Semi-Major Axis Length     = %ld\n", atol(argv[4]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		sprintf(MainProcessBuffer, "Semi-Minor Axis Length     = %ld\n", atol(argv[5]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		sprintf(MainProcessBuffer, "Number of Bins             = %ld\n", atol(argv[6]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		sprintf(MainProcessBuffer, "Number of Ball Droppings   = %ld\n", atol(argv[7]));
		write(1, MainProcessBuffer, strlen(MainProcessBuffer));		
		memset(MainProcessBuffer, 0, 10000);
		
		pid_t pid[4];
		//Fibbonacci
		if ((pid[0] = fork()) == 0) {
			write(1, "Fibonacci Process Created\n", 26);
			StartFibbonacciProcess(atol(argv[1]));
			
			exit(0);
		}
		
		//BuffonsNeedle
		if ((pid[1] = fork()) == 0) {
			write(1, "Buffon's Needle Process Created\n", 32);
			StartBuffonNeedleProcess(atol(argv[2]));
			
			exit(0);
		}

		//AreaOfEllipse
		if ((pid[2] = fork()) == 0) {
			write(1, "Ellipse Area Process Created\n", 29);
			StartAreaOfEllipseProcess(atol(argv[3]), atol(argv[4]), atol(argv[5]));
			
			exit(0);
		}	
		
		//PinbalGame
		if ((pid[3] = fork()) == 0) {
			write(1, "Pinball Process Created\n", 24);
			StartPinballGameProcess(atol(argv[6]), atol(argv[7]));
			
			exit(0);
		}
		
		//Main is now Waiting
		int status;
		write(1, "Main Process Waits\n", 19);
		
		for (int i = 0; i < 4; i++) {
			wait(&status);
		}
		
		//Main is now Exiting
		write(1, "Main Process Exits\n", 19);
	}
	
	return 1;

}