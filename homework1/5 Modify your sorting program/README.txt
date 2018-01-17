serial_merg_sort.c  - sorts the data without splitting it 
/**
 * Compile:
 * gcc serial_merg_sort_2halfs.c
 *
 * Usage:
 * ./a.out [number of runs]
 *
 */
 
 server_sort.c  - parallel sort
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
 
 client_sort.c - parallel sort 
  /**
 * This program works with conjunction with server_sort.c 
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