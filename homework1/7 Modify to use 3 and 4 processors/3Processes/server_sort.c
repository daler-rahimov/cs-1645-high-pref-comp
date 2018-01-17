/**
 * This program works with conjunction with client_sort.c
 * Compile:
 *  gcc server_sort.c sorting_lib.c -o server_sort
 *  gcc client_sort.c sorting_lib.c -o client_sort
 *
 * Run:(the server program has to be runed first):
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
	int sizeInBytes = (MAX_RAND_NUMS + 9) * sizeof(int);
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

	//set all the flags to 0 just in case
	randomArray[ID_SERVER_DATA_GENERATED_FOR1] = 0;
	randomArray[ID_CLIENT_1_SECOND_THIRD_SORTED] = 0;
	randomArray[ID_SERVER_FIRST_THIRD_SORTED] = 0;
	randomArray[ID_CLIENT_1_CONNECTED] = 0;
	randomArray[ID_SERVER_START_SORTING] = 0;
	randomArray[ID_CLIENT_2_CONNECTED] = 0;
	randomArray[ID_CLIENT_2_THIRD_THIRD_SORTED] = 0;
	randomArray[ID_SERVER_DATA_GENERATED_FOR2] = 0;

	int j;
	//wait till client1 connects
	while (randomArray[ID_CLIENT_1_CONNECTED] != 1)
		;
	//wait till client2 connects
	while (randomArray[ID_CLIENT_2_CONNECTED] != 1)
		;
	/*****START THE TIME *********/
	gettimeofday(&tvalBefore, NULL);
	// let every processes start sorting
	randomArray[ID_SERVER_START_SORTING] = 1;
	//Sort the first THIRD
	merge_sort(randomArray, 0, 999);
	//wait till second THIRD has been sorted
	while (randomArray[ID_CLIENT_1_SECOND_THIRD_SORTED] != 1)
		;
	//wait till THIRD THIRD has been sorted
	while (randomArray[ID_CLIENT_2_THIRD_THIRD_SORTED] != 1)
		;
	//Merge second 2 THIRDS
	merge(randomArray, 999, 1999, 2999);
	//Merge fist third with already sorted second 2 thirds
	merge(randomArray, 0, 999, 2999);

	/*******STOP THE TIME *********/
	gettimeofday(&tvalAfter, NULL);

	//print the data
//		for (i = 0; i < MAX_RAND_NUMS; i++) {
//			printf("%d \n", randomArray[i]);
//		}
//	} // main loop


	printf("Run time: 	%f milliSec\n", get_time_elap(&tvalAfter, &tvalBefore));
//	printf("The value of flag: %d\n", randomArray[FLAG_INDEX]);

	exit(0);
}

