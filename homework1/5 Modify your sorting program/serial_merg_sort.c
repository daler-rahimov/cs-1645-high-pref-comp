/**
 * Compile:
 * gcc serial_merg_sort_2halfs.c
 *
 * Usage:
 * ./a.out [number of runs]
 *
 */
/**
 * 5. Modify your sorting program to split the work between the server and client.
 * To split the work, have each program sort half of the data and then let one of them
 * merge the data together.  Compare the results to the serial sorting programs.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <float.h>
#include <limits.h>
#define MAX_RAND_NUMS 1000// have to be multiple of 2

int randomArray[MAX_RAND_NUMS];
struct timeval tvalBefore, tvalAfter;

/**
 * This functions records the time of the day
 * in to tvalBefore
 */
void start_time() {
	gettimeofday(&tvalBefore, NULL);
}
/**
 * This functions records the time of the day
 * in to tvalAfter
 */
void stop_time() {
	gettimeofday(&tvalAfter, NULL);
}

/**
 * This functions returns the time in milliseconds
 * from call of start_time to stop_time
 */
double get_time_elap() {
	   return (((tvalAfter.tv_sec - tvalBefore.tv_sec) * 1000000) +
	            (double)(tvalAfter.tv_usec - tvalBefore.tv_usec))/1000;
}
/**
 * This functions returns the time in microseconds
 * from call of start_time to stop_time
 */
long get_time_elap1() {
	return (tvalAfter.tv_usec - tvalBefore.tv_usec);
}


// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		} else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	 are any */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	 are any */
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

/* l is for left index and r is right index of the
 sub-array of arr to be sorted */
void merge_sort(int arr[], int l, int r) {
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l + (r - l) / 2;

		// Sort first and second halves
		merge_sort(arr, l, m);
		merge_sort(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}

int main(int argc, char** argv) {
	int numberOfRuns;
	double totalRunTime=0, minRunTime=LONG_MAX, maxRunTime=0,avgRunTime;
	if (argc < 2) {
		printf("Usage: ./a.out [number of runs]\n");
		exit(1);
	} else {
		numberOfRuns = atoi(argv[1]);
	}

	//gen random array
	int i;
	srand(time(NULL));
	for (i = 0; i < MAX_RAND_NUMS; i++) {
		randomArray[i] = rand();
	}

	for (i = 0; i < numberOfRuns; i++) {

		//copy it over to arrayToSort[] so we can sort it over and over
		//and it does not effect the runtime
		int arrayToSort[MAX_RAND_NUMS];
		int j;
		for (j = 0; j < MAX_RAND_NUMS; j++) {
			arrayToSort[j] = randomArray[j];
		}

		start_time();
		merge_sort(arrayToSort, 0, MAX_RAND_NUMS-1);
		stop_time();
		totalRunTime += get_time_elap();
		if (get_time_elap() < minRunTime)
			minRunTime = get_time_elap();
		if (get_time_elap() > maxRunTime)
			maxRunTime = get_time_elap();
	}
	avgRunTime = totalRunTime / numberOfRuns;

	printf("Number of ran ints: %d\n", MAX_RAND_NUMS);
	printf("Number of runs: 	%d\n", numberOfRuns);
	printf("Total run time: 	%f milliSec\n", totalRunTime);
	printf("Average run time: 	%f milliSec\n", avgRunTime);
	printf("Max run time: 		%f milliSec\n", maxRunTime);
	printf("Min run time: 		%f milliSec\n", minRunTime);

	return 0;
}
