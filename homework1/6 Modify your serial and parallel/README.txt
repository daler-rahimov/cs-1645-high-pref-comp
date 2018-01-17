serial_merg_sort.c  - (TO CHANGE THE DATA SIZE TO BE SORTED CHANGE MAX_RAND_NUMS IN THE FILE)
/**
 * Compile:
 * gcc serial_merg_sort_2halfs.c
 *
 * Usage:
 * ./a.out [number of runs]
 *
 */
 
 server_sort.c  - (TO CHANGE THE DATA SIZE CHANGE MAX_RAND_NUMS IN sorting_lib.h)
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
 * 6. Modify the serial and parallel programs to sort 1,000 2,000 to 30,000.  
 * Plot the timings.  Is there a linear scale?
 */
