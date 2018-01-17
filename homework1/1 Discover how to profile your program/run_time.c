/**
 * 1. Discover how to profile your program.  One potential way is to make use of gettimeofday.
 *   This will involve understanding that you are trying to time only a portion of your
 *   program and not the entire program.  Write a small program that will show how you can
 *   time a segment of your program.  Make the program such that it will let you do it many
 *   times and then produce an average of the result.
 */
/**
 * Usage: ./a.out [number of runs]
 * Ex:
 *./a.out 100
 	 Number of runs: 	100
 	 Total run time: 	93.000000 micSec
 	 Average run time: 	0.930000 micSec
 	 Max run time: 		1.000000 micSec
 	 Min run time: 		0.000000 micSec
 *
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <float.h>
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
 * This functions returns the time in microseconds
 * from call of start_time to stop_time
 */
double get_time_elap() {
	return (tvalAfter.tv_usec - tvalBefore.tv_usec);
}

int main(int argc, char** argv) {
	int numberOfRuns;
	double avgRunTime;
	double totalRunTime = 0, minRunTime = DBL_MAX, maxRunTime = 0;
	if (argc < 2) {
		printf("Usage: ./a.out [number of runs]\n");
		exit(1);
	} else {
		numberOfRuns = atoi(argv[1]);
	}

	int i;
	for (i = 0; i < numberOfRuns; i++) {
		start_time();
		// do some work that you what to get runtime for
		int j;
		for (j = 0; j < 100; ++j) {
			// dummy loop
		}
		stop_time();
		totalRunTime += get_time_elap();
		if (get_time_elap() < minRunTime)
			minRunTime = get_time_elap();
		if (get_time_elap() > maxRunTime)
			maxRunTime = get_time_elap();
	}
	avgRunTime = totalRunTime / numberOfRuns;

	printf("Number of runs: 	%d\n", numberOfRuns);
	printf("Total run time: 	%f micSec\n", totalRunTime);
	printf("Average run time: 	%f micSec\n", avgRunTime);
	printf("Max run time: 		%f micSec\n", maxRunTime);
	printf("Min run time: 		%f micSec\n", minRunTime);

	return 0;
}
