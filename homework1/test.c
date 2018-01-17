#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(){

	srand (time(NULL));
	int random = rand();
	printf("%d", random);


	return 0;
}
