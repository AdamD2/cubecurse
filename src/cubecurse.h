/*
 * Cubecurse is a simple ncurses based timer developed for timing Rubik's Cube
 * solves.
 *
 * Author: Adam Douglas <adam_douglas2@hotmail.com>
 * Github: https://github.com/adamd2/cubecurse
 */

// Libraries
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "time_list/Time_list.h"

// Definitions
// Color
#define BACKGROUND COLOR_BLACK
// Windows
#define NWINDOWS 6
#define TITLE 0
#define SCRAMBLE 1
#define HISTORY 2 
#define TIMER 3
#define STATS 4
#define CONTROLS 5
// Pbs
#define AO1 0
#define AO5 1
#define AO12 2
#define AO100 3
#define PB_NUM 4
// Controls
#define CONTROL_LEN 22

// Macros
#define init_color() if (has_colors()) {\
					     start_color();\
						 init_pair(1, COLOR_RED,     BACKGROUND);\
						 init_pair(2, COLOR_GREEN,   BACKGROUND);\
						 init_pair(3, COLOR_BLUE,    BACKGROUND);\
						 init_pair(4, COLOR_BLACK,   COLOR_GREEN);\
					 }

#define clear_windows() wclear(stdscr);\
                        wclear(ws[TITLE]);\
                        wclear(ws[SCRAMBLE]);\
                        wclear(ws[HISTORY]);\
                        wclear(ws[TIMER]);\
                        wclear(ws[STATS]);\
                        wclear(ws[CONTROLS])

#define refresh_windows() wrefresh(ws[TITLE]);\
                          wrefresh(ws[SCRAMBLE]);\
                          wrefresh(ws[HISTORY]);\
                          wrefresh(ws[TIMER]);\
                          wrefresh(ws[STATS]);\
                          wrefresh(ws[CONTROLS])

#define add_boxes() box(ws[TITLE], 0, 0);\
                    box(ws[SCRAMBLE], 0, 0);\
                    box(ws[HISTORY], 0, 0);\
                    box(ws[TIMER], 0, 0);\
                    box(ws[STATS], 0, 0);\
                    box(ws[CONTROLS], 0, 0)

#define delete_windows() delwin(ws[TITLE]);\
                         delwin(ws[SCRAMBLE]);\
                         delwin(ws[HISTORY]);\
                         delwin(ws[TIMER]);\
                         delwin(ws[STATS]);\
                         delwin(ws[CONTROLS])

// Global variables
char* scramble_options[18] = {"F ", "F' ", "F2 ", "B ", "B' ", "B2 ",
                              "L ", "L' ", "L2 ", "R ", "R' ", "R2 ",
                              "U ", "U' ", "U2 ", "D ", "D' ", "D2 "};
char* title_string = "Cubecurse v0.1";
char* control_button[7] = {"q", "n", "SPC", "U/D", "d", "f", "+"};
char* control_exp[7] = {"Quit", "New Scramble", "Start/Stop Timer", 
	"Navigate Times", "Delete", "DNF", "Add 2 Seconds"};
int control_num = 7;

// Functions
Time_list time_data_setup();
void print_time_data(WINDOW* history, Time_list time_data, int y);
void print_stats(WINDOW* stats, F_Time_list pbs[], F_Time_list recent[]);
void pb_recent_setup(F_Time_list pbs[], F_Time_list recent[]);
void calculate_stats(WINDOW* stats, Time_list time_data, F_Time_list pbs[],
					 F_Time_list recent[]);
void calculate_stats_all(WINDOW* stats, Time_list time_data, F_Time_list pbs[],
						 F_Time_list recent[]);
void generate_scramble(char cur_scramble[]);
void print_controls(WINDOW* controls, int y_res, int x_res);
void main_loop(int x_res[], int y_res[], int x_pos[], int y_pos[], WINDOW* ws[],
			   Time_list time_data, F_Time_list pbs[], F_Time_list recent[],
			   char cur_scramble[]);
void resize(int new_x, int new_y, int x_res[], int y_res[], int x_pos[], 
            int y_pos[], WINDOW* ws[], char cur_scramble[]);
int stop_timer(char cur_scramble[], WINDOW* scramble, WINDOW* history,
               Time_list time_data, F_Time_list recent[], int msec, int y);
int update_timer(struct timespec start);

