/**
 * This program works with conjunction with client_sort.c
 * Compile:
 *  gcc server_sort.c sorting_lib.c -o server_sort
 *  gcc client_sort.c sorting_lib.c -o client_sort
 *
 * Run:(the server program has to be run first):
 * 	./server_sort [number of runs]
 *	./client_sort
 */

/**
 * 5. Modify your sorting program to split the work between the server and client.
 * To split the work, have each program sort half of the data and then let one of
 * them merge the data together.  Compare the results to the serial sorting programs.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "sorting_lib.h"
struct timeval tvalBefore, tvalAfter;

int main(int argc, char** argv) {
	int numberOfRuns;
	double totalRunTime = 0, minRunTime = LONG_MAX, maxRunTime = 0, avgRunTime;
	if (argc < 2) {
		printf("Usage: ./a.out [number of runs]\n");
		exit(1);
	} else {
		numberOfRuns = atoi(argv[1]);
	}

	//gen random array not in shared memory (to make sure in every run same data is sorted)
	int i, randomArrNotShared[MAX_RAND_NUMS];
	srand(time(NULL));
	for (i = 0; i < MAX_RAND_NUMS; i++) {
		randomArrNotShared[i] = rand();
	}

	char c;
	int shmid;
	key_t key;
	char *shm;
	int *randomArray; // shared memory array

	/*
	 * We'll name our shared memory segment
	 * "12".
	 */
	key = 12;

	/*
	 * Create the segment.
	 */
	int sizeInBytes = (MAX_RAND_NUMS + 6) * sizeof(int);
//	printf("%d\n", sizeInBytes);
	if ((shmid = shmget(key, sizeInBytes, IPC_CREAT | 0666)) < 0) { // +6 is for a flags
		perror("shmget");
		exit(1);
	}

	/*
	 * Now we attach the segment to our data space.
	 */
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}
	randomArray = (int*) shm;

	// copy the value of number of run to shared memory
	randomArray[INDEX_OF_NUM_RUNS] = numberOfRuns;
	//set all the flags to 0 just in case
	randomArray[ID_SERVER_DATA_GENERATED] = 0;
	randomArray[ID_CLIENT_SECOND_HALF_SORTED] = 0;
	randomArray[ID_SERVER_FIRST_HALF_SORTED] = 0;
	randomArray[ID_CLIENT_1_CONNECTED] = 0;
	randomArray[ID_SERVER_START_SORTING] = 0;

	int j;
	for (j = 0; j < numberOfRuns; j++) { // main loop to sort the data over and over to make sure the result is not affected by other variables
		//	 * Copy random array over to shared memory.
		for (i = 0; i < MAX_RAND_NUMS; i++) {
			randomArray[i] = randomArrNotShared[i];
		}
		randomArray[ID_SERVER_DATA_GENERATED] = 1; //
		//wait till client connects
		while (randomArray[ID_CLIENT_1_CONNECTED] != 1)
			;
		randomArray[ID_CLIENT_1_CONNECTED] = 0; // set it back to next run
		/*****START THE TIME *********/
		gettimeofday(&tvalBefore, NULL);
		// let every processes start sorting
		randomArray[ID_SERVER_START_SORTING] = 1;
		//Sort the first half
		merge_sort(randomArray, 0, (MAX_RAND_NUMS / 2) - 1);

		//wait till second half has been sorted
		while (randomArray[ID_CLIENT_SECOND_HALF_SORTED] != 1)
			;
		randomArray[ID_CLIENT_SECOND_HALF_SORTED] = 0;

		//Merge two half
		merge(randomArray, 0, ((MAX_RAND_NUMS - 1) / 2), MAX_RAND_NUMS - 1);

		/*******STOP THE TIME *********/
		gettimeofday(&tvalAfter, NULL);

		totalRunTime += get_time_elap(&tvalAfter, &tvalBefore);
		if (get_time_elap(&tvalAfter, &tvalBefore) < minRunTime)
			minRunTime = get_time_elap(&tvalAfter, &tvalBefore);
		if (get_time_elap(&tvalAfter, &tvalBefore) > maxRunTime)
			maxRunTime = get_time_elap(&tvalAfter, &tvalBefore);

		//print the data
//		for (i = 0; i < MAX_RAND_NUMS; i++) {
//			printf("%d \n", randomArray[i]);
//		}
	} // main loop

	avgRunTime = totalRunTime / numberOfRuns;

	printf("Number of ran ints: %d\n", MAX_RAND_NUMS);
	printf("Number of runs: 	%d\n", numberOfRuns);
	printf("Total run time: 	%f milliSec\n", totalRunTime);
	printf("Average run time: 	%f milliSec\n", avgRunTime);
	printf("Max run time: 		%f milliSec\n", maxRunTime);
	printf("Min run time: 		%f milliSec\n", minRunTime);

//	printf("The value of flag: %d\n", randomArray[FLAG_INDEX]);

	exit(0);
}

