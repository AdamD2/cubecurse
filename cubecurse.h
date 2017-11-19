/*
 * Cubecurse is a simple ncurses based timer developed for timing Rubik's Cube
 * speedsolves.
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
#include "Time_list.h"

// Definitions
#define BACKGROUND COLOR_BLACK
#define TITLE 0
#define SCRAMBLE 1
#define HISTORY 2 
#define TIMER 3
#define STATS 4
#define CONTROLS 5

// Macros
#define clear_windows() wclear(stdscr);\
                        wclear(title);\
                        wclear(scramble);\
                        wclear(history);\
                        wclear(timer);\
                        wclear(stats);\
                        wclear(controls)

#define refresh_windows() wrefresh(title);\
                          wrefresh(scramble);\
                          wrefresh(history);\
                          wrefresh(timer);\
                          wrefresh(stats);\
                          wrefresh(controls)

#define add_boxes() box(title, 0, 0);\
                    box(scramble, 0, 0);\
                    box(history, 0, 0);\
                    box(timer, 0, 0);\
                    box(stats, 0, 0);\
                    box(controls, 0, 0)

#define delete_windows() delwin(title);\
                         delwin(scramble);\
                         delwin(history);\
                         delwin(timer);\
                         delwin(stats);\
                         delwin(controls)

// Global variables
char* scramble_options[18] = {"F ", "F' ", "F2 ", "B ", "B' ", "B2 ",
                              "L ", "L' ", "L2 ", "R ", "R' ", "R2 ",
                              "U ", "U' ", "U2 ", "D ", "D' ", "D2 "};
char* title_string = "Cubecurse v0.1";
char* controls_string = "q - Quit  n - New Scramble  \
Space - Start/Stop Timer  Up/Down or k/j - Navigate Times  d - Delete\
  f - DNF  + - Add 2 Seconds";

// Functions
Time_list time_data_setup();
void print_time_data(WINDOW* history, Time_list time_data, int y);
void generate_scramble(char current_scramble[]);
void main_loop(int x_res[], int y_res[], int x_pos[], int y_pos[],
               WINDOW* title, WINDOW* scramble, WINDOW* history, WINDOW* timer,
               WINDOW* stats, WINDOW* controls, Time_list time_data,
               char current_scramble[]);
void resize(int new_x, int new_y, int x_res[], int y_res[], int x_pos[], 
            int y_pos[], WINDOW* title, WINDOW* scramble, WINDOW* history,
            WINDOW* timer, WINDOW* stats, WINDOW* controls, char cur_scram[]);
int stop_timer(char current_scramble[], WINDOW* scramble, WINDOW* history,
               Time_list time_data, int msec, int y);
int update_timer(clock_t start);

