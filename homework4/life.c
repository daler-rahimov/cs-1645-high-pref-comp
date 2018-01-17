/*
 * Daler Rahimov
 * Homework #4
 *
 * 		You are to determine if there is a correlation between the initial density of the
 *  population and the density after 100 turns of Conway’s Game of Life simulation using
 *  a 10x10 matrix.  You must use a random matrix as the starting point.  You should do
 *  10 iterations of each density.  Plot the initial density and the final density.
 *
 */
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/* dimensions of the screen */

#define BOARD_WIDTH	10
#define BOARD_HEIGHT	10

/* set everthing to zero */

void initialize_board(int board[][BOARD_HEIGHT]) {
	int i, j;

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++)
			board[i][j] = 0;
}

/* add to a width index, wrapping around like a cylinder */

int xadd(int i, int a) {
	i += a;
	while (i < 0)
		i += BOARD_WIDTH;
	while (i >= BOARD_WIDTH)
		i -= BOARD_WIDTH;
	return i;
}

/* add to a height index, wrapping around */

int yadd(int i, int a) {
	i += a;
	while (i < 0)
		i += BOARD_HEIGHT;
	while (i >= BOARD_HEIGHT)
		i -= BOARD_HEIGHT;
	return i;
}

/* return the number of on cells adjacent to the i,j cell */

int adjacent_to(int board[][BOARD_HEIGHT], int i, int j) {
	int k, l, count;

	count = 0;

	/* go around the cell */

	for (k = -1; k <= 1; k++)
		for (l = -1; l <= 1; l++)

			/* only count if at least one of k,l isn't zero */

			if (k || l)
				if (board[xadd(i, k)][yadd(j, l)])
					count++;
	return count;
}

void play(int board[][BOARD_HEIGHT]) {
	/*
	 (copied this from some web page, hence the English spellings...)

	 1.STASIS : If, for a given cell, the number of on neighbours is
	 exactly two, the cell maintains its status quo into the next
	 generation. If the cell is on, it stays on, if it is off, it stays off.

	 2.GROWTH : If the number of on neighbours is exactly three, the cell
	 will be on in the next generation. This is regardless of the cell's 		current state.

	 3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will
	 be off in the next generation.
	 */
	int i, j, a, newboard[BOARD_WIDTH][BOARD_HEIGHT];

	/* for each cell, apply the rules of Life */

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++) {
			a = adjacent_to(board, i, j);
			if (a == 2)
				newboard[i][j] = board[i][j];
			if (a == 3)
				newboard[i][j] = 1;
			if (a < 2)
				newboard[i][j] = 0;
			if (a > 3)
				newboard[i][j] = 0;
		}

	/* copy the new board back into the old board */

	for (i = 0; i < BOARD_WIDTH; i++)
		for (j = 0; j < BOARD_HEIGHT; j++) {
			board[i][j] = newboard[i][j];
		}
}

/* print the life board */

void print(int board[][BOARD_HEIGHT]) {
	int i, j;

	/* for each row */

	for (j = 0; j < BOARD_HEIGHT; j++) {

		/* print each column position... */

		for (i = 0; i < BOARD_WIDTH; i++) {
			printf("%c", board[i][j] ? 'x' : ' ');
		}

		/* followed by a carriage return */

		printf("\n");
	}
}

void get_random_board(int board_passed[][BOARD_HEIGHT], int const density) {
	int i, ran;
	int cur_density = 0;
	int *board = (int*) board_passed;

	srand(time(NULL));   // should only be called once
	for (i = 0; cur_density < density; i++) {
		ran = rand() % 2;
		if (ran == 1 && board[i] == 0) { // if current value in the board is NOT one. Untill board has given density it will keep running
			cur_density++;
			board[i] = 1;
//			printf("%d = %d  %d\n", cur_density, density,board[i]);
		}
		if (i >= BOARD_HEIGHT * BOARD_WIDTH) {
			i = 0;
		}
	}
}

int get_density(int board_passed[][BOARD_HEIGHT]) {
	int cur_density = 0, var;
	int *board = (int*) board_passed;
	for (var = 0; var < (BOARD_HEIGHT * BOARD_WIDTH); ++var) {
		if (board[var] == 1) {
			cur_density++;
		}
	}
	return cur_density;
}

/* main program */

int main(int argc, char *argv[]) {
	int initial_density,from_density, to_density, avg_final_density, i_10;
	int board[BOARD_WIDTH][BOARD_HEIGHT], i;

	MPI_Init(NULL, NULL); // // Initialize the MPI environment

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	switch (world_rank) {
	case 0:
		from_density = 1;
		to_density = 5;
		break;
	case 1:
		from_density = 5;
		to_density = 10;
		break;
	case 2:
		from_density = 10;
		to_density = 15;
		break;
	case 3:
		from_density = 15;
		to_density = 20;
		break;
	default:
		from_density = 20 + 4 * (world_rank-4);
		to_density = (20 + 4 * (world_rank-4))+4;
		break;
	}

	// for each density
	for (initial_density = from_density; initial_density <= to_density; initial_density++) {
		//10 itoration of each density
		avg_final_density = 0;
		printf("Initial Density: %d\n", initial_density);
		for (i_10 = 0; i_10 < 10; i_10++) {
			initialize_board(board);
			get_random_board(board, initial_density);

			/* play game of life 100 times */
			for (i = 0; i < 100; i++) {
//			print (board);
				play(board);
			}
			avg_final_density += get_density(board);
		}
		printf("Final Density:   %d\n\n", (avg_final_density / 10));
	}
	// Finalize the MPI environment.
	MPI_Finalize();
}
