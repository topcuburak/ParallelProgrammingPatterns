#include <stdio.h>	   //to include basic C library
#include <omp.h>       //to include open MP library
#include <stdlib.h>	   //to enable dynamic memory management 
#include <time.h>      //to include time library to generate randomness

// initially, I have created the number of threads, 
// and number of tosses for estimation of pi.
// pi is chosen as 10 million and nthreads is not assigned at first.
int nthreads;
long long int number_of_toss = 10000000;

// estimator function makes estimation for pi with using monte carlo methods. 
void estimator()
{
	// creates threads with omp parallel with amount of nthreads.
	// nthreads is increases from 1 to 16
	#pragma omp parallel num_threads(nthreads)
	{
		double *x; //pointer points to x axis of tosses
		double *y; //pointer points to y axis of tosses
	    srand(time(NULL)); // randomize seed
	    // x and y are arrays to store indexes of tosses.
	    // they are created with dynamic allocation since it is not possible
	    // to allocate above 1 million elements by using static allocation for gcc. 
		x = (double*) malloc(number_of_toss * sizeof(double));
		y = (double*) malloc(number_of_toss * sizeof(double));
		//numb_in_circ holds the tosses that is inside of the circle
		long long int numb_in_circ = 0;
		
		// Up to number of tosses, below loop generates random doubles 
		// between 0 and 1. 
		for (int toss = 0; toss < number_of_toss; toss++)
		{
			x[toss] = ((double)rand() / (double) RAND_MAX);
			y[toss] = ((double)rand() / (double) RAND_MAX);

			// each of these tosses will be checked whether they are 
			// inside of the circle or not. If they are inside, 
			// number of tosses in circle wil be incremented
			if ((x[toss]*x[toss]+y[toss]*y[toss])<1)
			{
				numb_in_circ++;
			}
		}
		// after random tosses are generated with amount of 10 million
		// pi value is calculated with the below formula after number of toss
		// and number in circle is converted to double format. 
		double pi_est = ((double)(numb_in_circ)*4)/(double)number_of_toss;
		// estimation printed. 
		printf("%lf\n",pi_est);
		// dynamically allocated memories are released. 
		free(x);
		free(y);
	}
	//all threads are finalized with the above curly bracket.
}

int main()
{
	// to trace execution times, clocks are counted.
	clock_t start, end;
	double cpu_time;
	
	for (int i = 1; i<=16; i++)
	{
		nthreads = i;		//try with [1,16] threads.
		start = clock();	//count the clocks between start and end.
		estimator();
		end = clock();
		//calculate the time spent into estimator function 
		cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
		// print the results for execution time and corresponding number of thread.
		printf("cpu_time %lf seconds for num of threads = %d\n ", cpu_time, nthreads);	
	}
	return 0;
}

