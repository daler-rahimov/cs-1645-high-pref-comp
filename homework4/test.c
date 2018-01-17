#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH	10
#define BOARD_HEIGHT	10

int main() {
	int dencity = 99;
	printf("dencity %d\n", dencity);
	int board_passed[10][10];
	int i, j;

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			board_passed[i][j] = 0;
	int ran;
	int cur_dencity = 0;
	int *board = (int*) board_passed;

	srand(time(NULL));   // should only be called once
	for (i = 0; cur_dencity < dencity; i++) {
		ran = rand() % 2;
		if (ran == 1 && board[i] == 0) { // if current value in the board is NOT one. Untill board has given dencity it will keep running
			cur_dencity++;
			board[i] = 1;
			printf("%d = %d  %d\n", cur_dencity, dencity,board[i]);
		}
		if (i >= BOARD_HEIGHT * BOARD_WIDTH) {
			i = 0;
		}
	}

	printf("\n");

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			printf("%d ", board[i]);

	printf("%d = %d \n", cur_dencity, dencity);
}
