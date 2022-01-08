#include <stdio.h> // to include basic c functions such as printf
#include <stdlib.h>
#include <time.h>	// to measure elapsed times for execution with different number of threads
#include <omp.h>	// to include openmp library

#define STB_IMAGE_IMPLEMENTATION //to be able to parse png images
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION //to be able to save data in png format
#include "stb_image_write.h"

//given Gaussian blurrin matrix coefficients
static int gbk[5][5] = {{1, 4, 7, 4, 1},
                        {4, 16, 26, 16, 4},
                        {7, 26, 41, 26, 7},
                        {4, 16, 26, 16, 4},
                        {1, 4, 7, 4, 1}};

//blur function
unsigned char blur_im_sub(unsigned char **arr, int row, int column, int number_of_thread)
{
	int val = 0;	// the centered pixel and around 24 pixel values are multiplied with
					// corresponding coefficient and collected
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            val += (gbk[i][j] * arr[i+(row-2)][j+(column-2)]); //parallelizing this blur here is not 
        }													//as beneficial as below 
    }													//even if I appliced atomic, single operations to lock val
    return (val / 273); //return the results by dividing sum with 273
}

//read data inside from the image array. 
//row specify height of the image
//column specify the width of the image
//png_name is the name for saving manupiulated image
void read_and_write_image_RGB(unsigned char *image, int row, int column, int channels, const unsigned char *png_name, int number_of_thread)
{
    unsigned char **red_arr; //2D arrays for store parse of 3 channel results from the image
    unsigned char **green_arr;
    unsigned char **blue_arr;

    red_arr = malloc(row * sizeof(*red_arr)); //dynamic allocation for each of the 2D threads
    green_arr = malloc(row * sizeof(*red_arr));
    blue_arr = malloc(row * sizeof(*red_arr));

    if((red_arr == NULL) || (blue_arr == NULL) || (green_arr == NULL)) //if allocations are not done
    {
        printf("Unable to allocate memory.\n");
    }

	#pragma omp parallel for num_threads(number_of_thread)
    for (int i = 0; i < row; i++)	//dynamic allocations amount of column size for each row 
    {
    	red_arr[i] = malloc(column);
    	green_arr[i] = malloc(column);
    	blue_arr[i] = malloc(column);
    }

    //copy the image data from the 3 different 2D arrays (can be realized as mapping and scanning)
	#pragma omp parallel for num_threads(number_of_thread) collapse(2) 
    for(int i = 0; i < row; i++) 
    {
   		for (int j = 0; j < column; j++)
    	{
    		red_arr[i][j] = image[(i*column*channels)+(j*channels)];  //since image data is serial, I need to parse each corresponding 
			green_arr[i][j] = image[(i*column*channels)+(j*channels)+1];	//channel pixel for each different 2D arrays
			blue_arr[i][j] = image[(i*column*channels)+(j*channels)+2];		//column*channels = row size
    	}
    
	}

	size_t img_size = column * row * channels;
	unsigned char *res = malloc(img_size);	//allocate result array amount of column * row * channels(3 in all case)

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2)	//I did not comput each of 2D arrays in the 
	for (int i = 2; i < row-2; i++)								//same loop to benefit from spatial locality feature of the shared mem and caches
	{
		for(int j = 2; j < column-2; j++)						//started from 2 since 0th and 1st row and columns will be held as same
		{														//guided scheduling results in very similar performance with static scheduling
														//since guided has more overhead compared to the static
    		res[(i*column*channels)+(j*channels)] = blur_im_sub(red_arr, i, j, number_of_thread);	
		}														//collapse(2) since 2 times nested for loop
	}					

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2) //the comments written for red_arr is valid for this 
	for (int i = 2; i < row-2; i++)
	{
		for(int j = 2; j < column-2; j++)
		{
			res[(i*column*channels)+(j*channels)+1] = blur_im_sub(green_arr, i, j, number_of_thread );	
		}
	}

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2) //the comments written for red_arr is valid for this 
	for (int i = 2; i < row-2; i++)
	{
		for(int j = 2; j < column-2; j++)
		{
			res[(i*column*channels)+(j*channels)+2] = blur_im_sub(blue_arr, i, j, number_of_thread);
		}
	}

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2) //the 0th and 1st dimensions of the resultant
	for (int i = 0; i < 2; i++)							// 1D sequential array is the same with the image's pixel values
	{											
		for(int j = 0; j < column; j++)								// this nested for loop for first and second row
		{
    		res[(i*column*channels)+(j*channels)] = image[(i*column*channels)+(j*channels)];	
    		res[(i*column*channels)+(j*channels)+1] = image[(i*column*channels)+(j*channels+1)];	
    		res[(i*column*channels)+(j*channels)+2] = image[(i*column*channels)+(j*channels+2)];	

		}
	}

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2)
	for (int i = row-2; i < row; i++)		// this nested for loop for the last 2 rows
	{
		for(int j = 0; j < column; j++)
		{
    		res[(i*column*channels)+(j*channels)] = image[(i*column*channels)+(j*channels)];	
    		res[(i*column*channels)+(j*channels)+1] = image[(i*column*channels)+(j*channels+1)];	
    		res[(i*column*channels)+(j*channels)+2] = image[(i*column*channels)+(j*channels+2)];	

		}
	}

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2)
	for (int i = 2; i < row-2; i++) // this nested for loop for the first 2 columns
	{
		for(int j = 0; j < 2; j++)
		{
    		res[(i*column*channels)+(j*channels)] = image[(i*column*channels)+(j*channels)];	
    		res[(i*column*channels)+(j*channels)+1] = image[(i*column*channels)+(j*channels+1)];	
    		res[(i*column*channels)+(j*channels)+2] = image[(i*column*channels)+(j*channels+2)];	

		}
	}

	#pragma omp parallel for schedule (guided) num_threads(number_of_thread) collapse(2)
	for (int i = 2; i < row-2; i++)  // this nested for loop for the last 2 columns
	{
		for(int j = column-2; j < column; j++)
		{
    		res[(i*column*channels)+(j*channels)] = image[(i*column*channels)+(j*channels)];	
    		res[(i*column*channels)+(j*channels)+1] = image[(i*column*channels)+(j*channels+1)];	
    		res[(i*column*channels)+(j*channels)+2] = image[(i*column*channels)+(j*channels+2)];	

		}
	}

	//release each allocated row for 3 - 2D arrays
	#pragma omp parallel for num_threads(number_of_thread)
	for (int i = 0; i < row; i++)
	{
		free(red_arr[i]);
		free(green_arr[i]);
		free(blue_arr[i]);
	}

	//release each allocated row for 2D arrays
	free(red_arr);
	free(green_arr);
	free(blue_arr);
    stbi_write_png(png_name, column, row, channels, res, column*channels);	//for saving result, png_name is the target name of the png
    					// row specifies height, column specifies width, res is the 1D sequential array that holds the pixel values. 
    free(res);		//release the allocated 1D res arraya
}

int main(void) 
{		//example pngs are in example_png directory
		//to try code, add your png to the example_png directory and update
		//png_name and blurred_png_name variables respectively
	const char *png_name = "example_pngs/blurred_forest4.png";
	const char *blurred_png_name = "example_pngs/blurred_forest5.png";

	int column, row, channels;
	// to load png image pointed by 1D char array
	unsigned char *image = stbi_load(png_name, &column, &row, &channels, 3);

	struct timespec begin, end; 	//to record the begin and end time of the function
    for (int i=1; i<17; i++)
    {
   	    clock_gettime(CLOCK_REALTIME, &begin);		//record starting time
		read_and_write_image_RGB(image, row, column, 3, blurred_png_name, i);  //our function
   		clock_gettime(CLOCK_REALTIME, &end);		//record time of returning from MCS function
   		long sec = end.tv_sec - begin.tv_sec;		//elapsed time in terms of second
   		long nanosec = end.tv_nsec - begin.tv_nsec;	//elapsed time in terms of nanosecond
   		double elapsed = sec + nanosec*0.000000001;	
		printf("The elapsed time to run with %d amount of thread is %.3f seconds\n",i, elapsed);	//results
    }
    stbi_image_free(image); //release allocated image
	return 0;
}