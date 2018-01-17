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
struct timeval *tvalBefore, *tvalAfter;

int main() {
	char c;
	int shmid;
	key_t key;
	char *shm;
	int *randomArray;

	/*
	 * We'll name our shared memory segment
	 * "1234".
	 */
	key = 1234;

	/*
	 * Create the segment.
	 */
	int sizeInBytes = (MAX_RAND_NUMS + 1) * sizeof(int)+ sizeof(tvalBefore)+ sizeof(tvalAfter);
	printf("%d\n", sizeInBytes);
	if ((shmid = shmget(key, sizeInBytes, IPC_CREAT | 0666)) < 0) { // +1 is for a flag
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

	/*
	 * Generate random data in shared memory.
	 * Now put some things into the memory for the
	 * other process to read.
	 */
	randomArray = (int*) shm;

	// set time value pointers (values are stored after flags)
	tvalBefore = (struct timeval*) randomArray[TIMEVALS_INDEX];
	tvalAfter = tvalBefore+1;

	//gen random array
	int i;
	srand(time(NULL));
	for (i = 0; i < MAX_RAND_NUMS; i++) {
		randomArray[i] = rand();
	}

	randomArray[FLAG_INDEX] |= DATA_HAS_BEEN_GENERATED; //

	/**
	 * wait till client gets connected
	 */
//	while ((randomArray[FLAG_INDEX] & CLIENT_HAS_CONNECTED) ==0)
//		sleep(1);
	/**
	 * Sort the first half
	 */
	merge_sort(randomArray, 0, (MAX_RAND_NUMS / 2) - 1);

	/*
	 * Finally, we wait until the other process
	 * changes the first character of our memory
	 * to '*', indicating that it has read what
	 * we put there.
	 */
//	while ((randomArray[FLAG_INDEX] & SECOND_HALF_HAS_BEEN_SORTED)==0)
//		sleep(1);

	/**
	 * Merge two halfs
	 */
	merge(randomArray, 0, ((MAX_RAND_NUMS - 1) / 2), MAX_RAND_NUMS - 1);

	/*******STOP THE TIME *********/
	stop_time(tvalAfter);
	printf("Run time %d\n", get_time_elap(tvalBefore,tvalAfter));

	//print the data
	for (i = 0; i < 10; i++) {
		printf("%d \n", randomArray[i]);
	}
	printf("The value of flag: %d\n", randomArray[FLAG_INDEX]);

	exit(0);
}

