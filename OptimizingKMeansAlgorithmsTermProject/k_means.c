#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>	// to include openmp library

#define NUM_OF_POINTS 250000 	//number of points

#define NUM_OF_CLUSTERS 5   //number of clusters

struct Centroid    //cluster structure; x,y coordinates and cluster number
{
	float x,y;
	int cluster;
};

struct Point 		//point structure; x,y coordinates, distance to its cluster and belonging cluster
{					//number
	float x, y;
	int cluster;
	float distance;
};

void initialize_points(struct Point* points)  // initialize the data set where x and y coordinates are in between [0, 1000000] 
{
	srand(time(0));		//random initializer 
    #pragma omp parallel for num_threads (12) schedule(guided, 500)	//mapped with 12 threads and guided scheduling where chunk size is 500byte
	for (int i = 0; i < NUM_OF_POINTS ; i++)
	{
		points[i].x = (rand() % 1000001);
		points[i].y = (rand() % 1000001);
	}
}


void initialize_centroids(struct Centroid *centroids) 	//similar to the initialization of the dataset.
{
	srand(time(0)); 				//initialize NUM_CLUSTER number of clusters
    #pragma omp parallel for num_threads (12) schedule(guided, 500)
	for (int i = 0; i < NUM_OF_CLUSTERS ; i++)
	{
		centroids[i].x = (rand() % 1000001);
		centroids[i].y = (rand() % 1000001);
		centroids[i].cluster = i; 		//cluster identity number
	}	
}

void Assigning_points_to_clusters(struct Point *points, struct Centroid *clusters) 	//assigning points to the clusters 
{
	#pragma omp target data map(to:clusters[0:NUM_OF_CLUSTERS], points[0:NUM_OF_POINTS]) //map specifier for the data pointer by points and clusters
	{	
	    #pragma omp parallel for num_threads (12) schedule(guided, 500) 	//initialize 12 threads for parallel mapping
		for (int i = 0; i < NUM_OF_POINTS; i++)
		{
			float distance[NUM_OF_CLUSTERS];   //generate NUM_CLUSTERs amount distance for each point
			for (int j = 0; j < NUM_OF_CLUSTERS; j++)
			{
				distance[j] = pow((points[i].x-clusters[j].x), 2) + pow((points[i].y-clusters[j].y), 2);  //measure distance between a point and each of the clusters
			}
			int smallest_ind = 0;
			float smallest_dist = distance[0];
			for (int j = 0; j < NUM_OF_CLUSTERS; j++)  //this finds the minimum distance and corresponding cluster
			{
				if (distance[j] < smallest_dist)
				{
					smallest_dist = distance[j];
					smallest_ind = j;
				}
			}
			points[i].cluster = smallest_ind;  //then updates the points' clusters 
			points[i].distance = smallest_dist;  //iteratively for each point.
		}
	}
}

void Update_centroids(struct Point *points, struct Centroid *clusters)  //after assigning points to the clusters, updates the cluster centers	
{
	#pragma omp parallel for num_threads (4) 		//initializes with 4 threads.
    for (int j = 0; j < NUM_OF_CLUSTERS; j++)		//for each cluster, add the coordinates of the each included points to update center
	{
		float center_x = 0;
		float center_y = 0;
		long int counter = 0;	
		#pragma omp parallel for num_threads(4) reduction(+:center_x, center_y, counter)  //reduction on addition of center coordinates and counter. 
		for (int i = 0; i < NUM_OF_POINTS; i++)
		{
			if (points[i].cluster == j)
			{
					center_x += points[i].x;
					center_y += points[i].y;
					counter++;
			}
		}
		clusters[j].x = center_x/counter;    // divides by counter for added center variables to update center for x and y coordinates
		clusters[j].y = center_y/counter;		
	}
}

void SaveToCSV(char *file_name_points, char *file_name_centroids, struct Point *points, struct Centroid *centroids)
{							//saves results to csv file to record and plot them in other code.
	FILE *file_ptr;
	file_ptr = fopen(file_name_points, "w+"); 		//opens a file with a specified name 
	fprintf(file_ptr,"X, Y, cluster\n");  		//write first line to X, Y and Cluster identity
 	#pragma omp parallel for num_threads (12) schedule(guided, 500)   // then distribute this write process to 12 threads
	for (int i=0; i < NUM_OF_POINTS ; i++)
	{
		fprintf(file_ptr,"%f, %f, %d\n", points[i].x, points[i].y, points[i].cluster);  //writes x and y coordinates and corresponding clusters
	}
	fclose(file_ptr); //closes the file

	FILE *file_ptr2;
	file_ptr2 = fopen(file_name_centroids, "w+");  //opens  a csv file to hold clusters
	fprintf(file_ptr2,"X, Y, cluster\n");
	#pragma omp parallel for num_threads (NUM_OF_CLUSTERS) schedule(guided, 500)
	for (int i=0; i < NUM_OF_CLUSTERS ; i++)
	{
		fprintf(file_ptr2,"%f, %f, %d\n", centroids[i].x, centroids[i].y, centroids[i].cluster); //similarly, records the x,y and cluster identity number
	}
	fclose(file_ptr2);  //closes the file
}


int main()
{
	struct timespec begin, end; 	//to record the begin and end time of the function
    clock_gettime(CLOCK_REALTIME, &begin);		//record starting time

	struct Point *points = (struct Point*) malloc(NUM_OF_POINTS*sizeof(struct Point));   //allocate NUM_OF_POINTS amount point structure pointed
																						//by points
	struct Centroid *centroids = (struct Centroid*) malloc(NUM_OF_CLUSTERS*sizeof(struct Centroid));  //allocate NUM_OF_CLUSTERS amount cluster structure pointed
																						//by centroids
	initialize_points(points);		//then, initialize the points and centroids
	initialize_centroids(centroids);
	Assigning_points_to_clusters(points, centroids); 		//and assign points to the clusters and update the centroids firstly.
	Update_centroids(points, centroids);

	int counter = 0;			//then do this re-assigning and updates for 1000 times.
	while (counter < 1000)
	{
		Assigning_points_to_clusters(points, centroids);
		Update_centroids(points, centroids);
		counter++;
	}
	SaveToCSV("points.csv", "clusters.csv", points, centroids); 		//then record the results

	clock_gettime(CLOCK_REALTIME, &end);		//record time of returning from MCS function
   	long sec = end.tv_sec - begin.tv_sec;		//elapsed time in terms of second
   	long nanosec = end.tv_nsec - begin.tv_nsec;	//elapsed time in terms of nanosecond
   	double elapsed = sec + nanosec*0.000000001;	
	printf("The elapsed time to run is %.3f seconds\n", elapsed);	//results

	free(points);		//release the allocated memories for points and clusters
	free(centroids);
	return 0;
}
