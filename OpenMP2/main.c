#include <stdio.h>	//to include basic c functions
#include <stdlib.h>
#include <time.h>	//to seed the random number generator and calculate the 
					//elapsed time during the run with using timespec structure
#include <omp.h>	//to include openmp lib

#define array_size 320000 // 1D array size

int * random_numbs_generator(int number_of_thread) //random number generator function
{
	srand(time(0));
	static int arr[array_size];	//since the allocation amount is static, I have used all arrays as static
								//for callling and returning as pointer
	#pragma omp parallel for num_threads(number_of_thread)  //create number_of_thread amount of thread 
    for(int i = 0; i<array_size; i++)						//to generate 1D input array
    {
    	arr[i] = -15+rand()%31;		//from -15 to 15
    }
    return arr;  //return the generated array
}

void printer_funct(int *arr)	//this function created as void is just used to check for s, m, s_m and index arrays
{								//whether they are calculated as true or not
	for (int i = 0; i < array_size; i++)
	{
		printf("%d ",arr[i]);	//prints each element of the given array
	}
	printf("\n");
}

void print_results(int *index, int *s_m)	//this function will find MCS value and corresponding sub array interval of MCS 
{
	int max = s_m[0];	// MCS 	
	int ind = 0;	// lower index 
	int max_ind = 0; 	//upper index
	for (int i = 1; i < array_size; i++)
	{
		if(s_m[i] > max)	//to find maximum element of s_m
		{
			max = s_m[i];	//holds the maximum of the s_m 
			max_ind = i;	//hold the corresponding index of the max element
		}
		if ((index[i] > ind) && (index[i] <= max_ind))
		{
			ind = index[i];	// to find maximum of the index element lower than the max_ind
		}
	}
	if ((max_ind == 0) && (ind == 0) && (s_m[0]>0))	//if MCS is just the 0th location
	{
		ind--;	//decrement ind from 0 to -1
	}
	else if ((max_ind != 0) && (ind == 0) && (s_m[0]>0))	//If MCS starts from the 0th location, include the 0th element
	{
		ind--;			//decrease ind from 0 to -1.
		max += s_m[0];		//add 0th element to the MCS
	}
	printf("MCS is %d and the consecutive sub-array is [%d, %d]\n",max, ind+1, max_ind);	
}

int MCS(int *x, int num_of_thread)	//this function calculates the necessary arrays s, m, s_m, index
{									//x is the 1D array, num_of_thread is the number of threads
	static int s[array_size];		//s is the same of the pseudo code. 	 
	s[0] = *x;						//0th element initialized as 0th element of the input array
	#pragma omp parallel for num_threads(num_of_thread)		//omp parallel for statement with num_of_thread amount threads
	for (int i = 1; i<array_size; i++)
	{
		s[i] = *(x+i)+s[i-1];		// s[i] = s[i-1] + x[i]
	}

	static int m[array_size];		// m array as in the homework  
	m[0] = s[0];		// m[0] is the x[0]
	#pragma omp parallel for num_threads(num_of_thread) //omp parallel for statement with num_of_thread amount threads
	for (int i = 1; i<array_size; i++)	
	{
		if (s[i]< m[i-1])		// calculate m[i] with min prefix of s[i]<m[i-1]  
		{
			m[i] = s[i];
		}
		else 		// s[i] >= m[i-1] -> m[i] = m[i-1]
		{
			m[i] = m[i-1];
		}
	}

	static int s_m[array_size];		//s_m array as in the homework that holds the sums
	s_m[0] = s[0];		// s_m[0] = s[0]	
	#pragma omp parallel for num_threads(num_of_thread) //omp parallel for statement with num_of_thread amount threads
	for (int i = 1; i<array_size; i++)
	{
		s_m[i] = s[i] - m[i-1];	// s_m[i] = s[i] - m[i-1] as the same in the homework manual
	}
	
	static int index[array_size];	//index array used to calculate sub array intervar
	index[0] = 0;	
	#pragma omp parallel for num_threads(num_of_thread)	//omp parallel for statement with num_of_thread amount threads
	for (int i = 0; i < array_size; i++)	// scan where i<=j 
	{										// j traces the m array and i traces the s array
		for (int j = i; j < array_size; j++)
		{
			if(s[i] == m[j])
			{
				index[j] = i;	//update indices where, s[i] = m[j]
			}
		}
	}
	print_results(index, s_m);	//send to the print results function with the calculated index and s_m arrays
}

int MCS_test(int *x)	//this function is just created to compare the results between 
{						//parallelized code and sequential code.
	static int s[array_size];	//I want to wonder about the array limits of each thread
	s[0] = *x;					//to verify they do not effect each other, 
	for (int i = 1; i<array_size; i++)		// I want to implement sequential of the above function 
	{									//as MCS_test that computes the same thing, without any thread.
		s[i] = *(x+i)+s[i-1];			//so above comments are valid for this function
	}

	static int m[array_size];
	m[0] = s[0];
	for (int i = 1; i<array_size; i++)
	{
		if (s[i]< m[i-1])
		{
			m[i] = s[i];
		}
		else
		{
			m[i] = m[i-1];
		}
	}

	static int s_m[array_size];
	s_m[0] = s[0];
	for (int i = 1; i<array_size; i++)
	{
		s_m[i] = s[i] - m[i-1];
	}

	static int index[array_size];
	index[0] = 0;
	for (int i = 0; i < array_size; i++)
	{
		for (int j = i; j < array_size; j++)
		{
			if(s[i] == m[j])
			{
				index[j] = i;
			}
		}
	}
	print_results(index, s_m);
}

int main()
{
	struct timespec begin, end; 	//to record the begin and end time of the function with timespec structure 	
	int *x = random_numbs_generator(16);	//generate input array with 16 thread
	for (int i = 1; i<17; i++)
	{											
	    clock_gettime(CLOCK_REALTIME, &begin);		//record starting time
		MCS(x, i);			//x is the 1D input array (same for each iteration), i is the thread amount
    	clock_gettime(CLOCK_REALTIME, &end);		//record time of returning from MCS function
    	long sec = end.tv_sec - begin.tv_sec;		//elapsed time in terms of second
    	long nanosec = end.tv_nsec - begin.tv_nsec;	//elapsed time in terms of nanosecond
    	double elapsed = sec + nanosec*0.000000001;	
		printf("The elapsed time to run with %d threads is %.3f seconds\n", i, elapsed);	//results
	}
	MCS_test(x);	//verification such that resultant MCS and sub array interval are the same for both parallelized and sequential versions
	return 0;
}