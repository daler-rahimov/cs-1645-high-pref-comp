/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "sorting_lib.h"
struct timeval *tvalBefore, *tvalAfter;

main() {
	int numberOfRuns = 0;
	int shmid;
	key_t key;
	char *shm;
	int *randomArray;

	/*
	 * We need to get the segment named
	 * "5678", created by the server.
	 */
	key = 12;

	/*
	 * Locate the segment.
	 */
	int sizeInBytes = (MAX_RAND_NUMS + 9) * sizeof(int);
	if ((shmid = shmget(key, sizeInBytes, 0666)) < 0) {
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
	// get number of runs from shared memory
	numberOfRuns = randomArray[INDEX_OF_NUM_RUNS];

//	for (j = 0; j < numberOfRuns; j++) {
	randomArray[ID_CLIENT_1_CONNECTED] = 1;
	// wiat till the data has been generated by the server
	while (randomArray[ID_SERVER_START_SORTING] != 1)
		;
	/// sort second half
	merge_sort(randomArray, 7500, 1499);
	//set the flag
	randomArray[ID_CLIENT_1_SECOND_THIRD_SORTED] = 1;
//	}
	printf("Done!\n");
	exit(0);
}
