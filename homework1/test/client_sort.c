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
	int shmid;
	key_t key;
	char *shm;
	int *randomArray;

	/*
	 * We need to get the segment named
	 * "5678", created by the server.
	 */
	key = 1234;

	/*
	 * Locate the segment.
	 */
	int sizeInBytes = (MAX_RAND_NUMS + 1) * sizeof(int)+ sizeof(tvalBefore)+ sizeof(tvalAfter);
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

	// set time value pointers (values are stored after flags)
	tvalBefore = (struct timeval*) randomArray[TIMEVALS_INDEX];
	tvalAfter = tvalBefore+1;

	randomArray[FLAG_INDEX] = randomArray[FLAG_INDEX]|CLIENT_HAS_CONNECTED;
	/*****START THE TIME *********/
	start_time(tvalBefore);

	/// sort second half
	merge_sort(randomArray, (MAX_RAND_NUMS / 2), MAX_RAND_NUMS - 1);
	//set the flag
	randomArray[FLAG_INDEX] = SECOND_HALF_HAS_BEEN_SORTED |randomArray[FLAG_INDEX];
	exit(0);
}
