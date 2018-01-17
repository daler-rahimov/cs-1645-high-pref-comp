
#include "sorting_lib.h"  /* Include the header (not strictly necessary here) */

/**
 * This functions records the time of the day
 * in to tvalBefore
 */
void start_time(struct timeval* tvalBefore) {
	gettimeofday(tvalBefore, NULL);
}
/**
 * This functions records the time of the day
 * in to tvalAfter
 */
void stop_time(struct timeval* tvalAfter) {
	gettimeofday(tvalAfter, NULL);
}

/**
 * This functions returns the time in microseconds
 * from call of start_time to stop_time
 */
long get_time_elap(struct timeval* tvalBefore,struct timeval* tvalAfter) {
	return (tvalAfter->tv_usec - tvalBefore->tv_usec);
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
