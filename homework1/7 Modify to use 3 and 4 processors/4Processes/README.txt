
complie:
gcc server_sort.c sorting_lib.c  -o server_sort
gcc client_1_sort.c sorting_lib.c -o client_1_sort
gcc client_2_sort.c sorting_lib.c -o client_2_sort

run:
./server_sort
./client_1_sort
./client_2_sort