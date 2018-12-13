// Implements all functions declared in Scramble.h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Scramble.h"

// Global variables
const char moves[6] = {'F', 'B', 'L', 'R', 'U', 'D'};
const char variations[3] = {' ', '\'', '2'};

/*
 * Take an array of chars and put in a 3x3x3 scramble
 * Pre: cur_scramble is 60 chars long
 * Post: cur_scramble contains a 20 move scramble
 */
void generate_scramble(char* scramble) {
	int i = 0, pos = 0;
	int move = 0, prev_move, move_axis, variation;
	int lock[3] = {0, 0, 0};
	int continued = 0;

	while (i < 20) {
		// Generate a random move different to the previous one
		if (!continued) {
			prev_move = move;
		}
		move = (rand() % 6);
		while (move == prev_move) {
			move = (rand() % 6);
		}
		move_axis = ceil((move+1)/2) - 1;

		// Check axis locks
		if (lock[move_axis] == 2) {
			continued = 1;
			continue;
		} else {
			continued = 0;
			lock[move_axis]++;
			lock[(move_axis+4)%3] = 0;
			lock[(move_axis+5)%3] = 0;
		}

		// Generate a random option for a variation on the move and output
		variation = (rand() % 3);
		scramble[pos++] = moves[move];
		scramble[pos++] = variations[variation];
		scramble[pos++] = ' ';
		i++;
	}
}
