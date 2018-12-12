/*
 * Cubecurse is a simple ncurses based timer developed for timing Rubik's Cube
 * solves.
 *
 * Author: Adam Douglas <adam_douglas2@hotmail.com>
 * Github: https://github.com/adamd2/cubecurse
 */

#include "cubecurse.h"

int main(int argc, char* argv[]) {
    initscr();      // Initialize curses
    keypad(stdscr, TRUE);   // Enable keyboard mapping
    nonl();         // No NL->CR/NL on output
    cbreak();       // Take input one at a time
    nodelay(stdscr, TRUE);  // Do not pause on getch()
    curs_set(0);    // Make cursor invisible
    echo();         // Do not echo input

	// Initialise variables
    char cur_scramble[60];
    clock_t t = clock();
    srand((unsigned) time(&t)); // Seed rand

    // Set up some basic colors
	init_color();

    // Create window based variables
    int parent_x, parent_y, new_x, new_y;
    getmaxyx(stdscr, parent_y, parent_x);   // Get max dimensions
    int title_center = (parent_x - 2 - strlen(title_string)) / 2; // Center align
    int mid_y = parent_y - 3 - 3 - 4;

    int y_res[6] = {3, 3, mid_y, mid_y, mid_y, 4};
    int x_res[6] = {parent_x, parent_x, 15, 
                    parent_x-15-33, 33, parent_x};

    int mid_pos = y_res[TITLE]+y_res[SCRAMBLE];

    int y_pos[6] = {0, y_res[SCRAMBLE], mid_pos, mid_pos, mid_pos, 
                    parent_y-y_res[CONTROLS]};
    int x_pos[6] = {0, 0, 0, x_res[HISTORY], parent_x-x_res[STATS], 0};

    // Window setup
	WINDOW* ws[NWINDOWS];
    ws[TITLE] = newwin(y_res[TITLE], x_res[TITLE], y_pos[TITLE], x_pos[SCRAMBLE]);
    ws[SCRAMBLE] = newwin(y_res[SCRAMBLE], x_res[SCRAMBLE], y_res[SCRAMBLE], x_pos[SCRAMBLE]);
    ws[HISTORY] = newwin(y_res[HISTORY], x_res[HISTORY], y_pos[HISTORY], x_pos[HISTORY]);
    ws[TIMER] = newwin(y_res[TIMER], x_res[TIMER], y_pos[TIMER], x_pos[TIMER]);
    ws[STATS] = newwin(y_res[STATS], x_res[STATS], y_pos[STATS], x_pos[STATS]);
    ws[CONTROLS] = newwin(y_res[CONTROLS], x_res[CONTROLS], y_pos[CONTROLS], x_pos[CONTROLS]);

    // Time data setup
    Time_list time_data = time_data_setup();
    F_Time_list pbs[PB_NUM];
	F_Time_list recent[PB_NUM];
	pb_recent_setup(pbs, recent);

    // Initial printing
    add_boxes();
    mvwprintw(ws[TITLE], 1, title_center, title_string);
    generate_scramble(cur_scramble);
    mvwprintw(ws[SCRAMBLE], 1, 2, "Scramble: %s", cur_scramble);
	print_controls(ws[CONTROLS], y_res[CONTROLS], x_res[CONTROLS]);
    print_time_data(ws[HISTORY], time_data, y_res[HISTORY]);
    refresh_windows();

    // Main loop
    main_loop(x_res, y_res, x_pos, y_pos, ws, time_data, pbs, recent, cur_scramble);

    // Finish
    delete_windows();
    endwin();
    return EXIT_SUCCESS;
}

/* 
 * Initialise a list of time data
 */
Time_list time_data_setup() {
    Time_list l = new_list();

    // TODO load from file

    return l;
}

/*
 * Compare the length of the time list with the height of the screen and call
 * the relevant function from Time_list.h
 */
void print_time_data(WINDOW* history, Time_list time_data, int y) {
    int l = tl_length(time_data);

    if (l == 0) {
        // Do nothing
    } else if (l <= y-2) {
        print_up(history, time_data);
    } else {
        print_down(history, time_data, y-2);
    }
}

/*
 * Print the stats 
 */
void print_stats(WINDOW* stats, F_Time_list pbs[], F_Time_list recent[]) {
    // Print best time
    print_up_stats(stats, pbs[AO1], "Best time:", 1);
	print_up_stats(stats, recent[AO1], "Current time:", 2);
	print_up_stats(stats, pbs[AO5], "Best average of 5:", 3);
	print_up_stats(stats, recent[AO5], "Current average of 5:", 4);
	print_up_stats(stats, pbs[AO12], "Best average of 12:", 5);
	print_up_stats(stats, recent[AO12], "Current average of 12:", 6);
	print_up_stats(stats, pbs[AO100], "Best average of 100:", 7);
	print_up_stats(stats, recent[AO100], "Current average of 100:", 8);
}

/*
 * Load in previous pbs or create empty lists for them
 */
void pb_recent_setup(F_Time_list pbs[], F_Time_list recent[]) {
    pbs[AO1]		= new_f_list(1);
    pbs[AO5]		= new_f_list(5);
    pbs[AO12]		= new_f_list(12);
    pbs[AO100]		= new_f_list(100);
	recent[AO1]		= new_f_list(1);
	recent[AO5]		= new_f_list(5);
	recent[AO12]	= new_f_list(12);
	recent[AO100]	= new_f_list(100);

    // TODO load from file or calculate
}

/*
 * Update stats based on the most recent recorded time and display them in the
 * stats window
 */
void calculate_stats(WINDOW* stats, Time_list time_data, F_Time_list pbs[],
					 F_Time_list recent[]) {
    int len = tl_length(time_data);

    if (len >= 1) {
        calculate_average(recent[AO1], pbs[AO1]);
        if (len >= 5) {
            calculate_average(recent[AO5], pbs[AO5]);
            if (len >= 12) {
                calculate_average(recent[AO12], pbs[AO12]);
                if (len >= 100) {
                    calculate_average(recent[AO100], pbs[AO100]);
                }
            }
        }
    }
}

/*
 * Update stats based on all recorded times and display them in the stats window
 */
void calculate_stats_all(WINDOW* stats, Time_list time_data, F_Time_list pbs[],
						 F_Time_list recent[]) {
    int len = tl_length(time_data);
	// TODO
}

/*
 * Take an array of chars and put in a 3x3x3 scramble
 * Pre: cur_scramble is 60 chars long
 * Post: cur_scramble contains a 20 move scramble
 */
void generate_scramble(char cur_scramble[]) {
    int index = 0;
    int rand_num = 0, prev_rand;
    for (int i = 0; i < 20; i++) {
        prev_rand = rand_num;
        rand_num = rand() % 18;
        while (rand_num-(rand_num%3) == prev_rand-(prev_rand%3)) {
            rand_num = rand() % 18;
        }
        strcpy(&cur_scramble[index], scramble_options[rand_num]);
        index += strlen(scramble_options[rand_num]);
    }
    for (index; index < 60; index++) {
        strcpy(&cur_scramble[index], " ");
    }
}

/*
 * Print the program controls at the bottom of the screen
 */
void print_controls(WINDOW* controls, int y_res, int x_res) {
	int y = 1;
	int x = 2;
	int printed = 0;
	for (int i = 0; i < control_num; i++) {
		if (x + CONTROL_LEN < x_res) {
			mvwprintw(controls, y, x, "%-4s%-17s", control_button[i],
					  control_exp[i]);
			x += CONTROL_LEN;
		} else {
			y++;
			x = 2;
			if (y > y_res) {
				break;
			}
		}
	}
}

/* 
 * Main loop of the program
 */
void main_loop(int x_res[], int y_res[], int x_pos[], int y_pos[], WINDOW* ws[],
			   Time_list time_data, F_Time_list pbs[], F_Time_list recent[],
			   char cur_scramble[]) {
    // Resizing variables
    int parent_y, parent_x, new_y, new_x;
    getmaxyx(stdscr, parent_y, parent_x);

    // Timing variables
    int timing = FALSE;
    struct timespec start;
    int msec = 0;

    while (TRUE) {
        int c = getch();

        // Handle resizing
        getmaxyx(stdscr, new_y, new_x);
        if (new_y != parent_y || new_x != parent_x) {
            parent_y = new_y;
            parent_x = new_x;
            resize(new_x, new_y, x_res, y_res, x_pos, y_pos, ws, cur_scramble);
        }

        // Handle keyboard input
        if (c == 'n') {
			// Generate a new scramble
            generate_scramble(cur_scramble);
            mvwprintw(ws[SCRAMBLE], 1, 2, "Scramble: %s", cur_scramble);
        } else if (c == 'q') {
			// Quit the program
            break;
        } else if (c == ' ' && timing == FALSE) {
			// Start the timer
            timing = TRUE;
            clock_gettime(CLOCK_REALTIME, &start);
        } else if (c == ' ' && timing == TRUE) {
			// End the timer
            timing = stop_timer(cur_scramble, ws[SCRAMBLE], ws[HISTORY], 
								time_data, recent, msec, y_res[HISTORY]);
            calculate_stats(ws[STATS], time_data, pbs, recent);
            print_stats(ws[STATS], pbs, recent);
        } else if (c == 'f') {
			// Set DNF for the selected time
            change_dnf(time_data);
            print_time_data(ws[HISTORY], time_data, y_res[HISTORY]);
            calculate_average_all(time_data, pbs[AO1]);
            print_stats(ws[STATS], pbs, recent);
        } else if (c == '+') {
			// Set +2 for the selected time
            change_plus_two(time_data);
            print_time_data(ws[HISTORY], time_data, y_res[HISTORY]);
            calculate_average_all(time_data, pbs[AO1]);
            print_stats(ws[STATS], pbs, recent);
        }

		// Update the timer and print it
        if (timing) msec = update_timer(start);
        mvwprintw(ws[TIMER], y_res[TIMER]/2, x_res[TIMER]/2, "%d.%d", msec/1000, msec%1000);

        // Refresh windows
        add_boxes();
        refresh_windows();
    }
}

/*
 * Handle the event of the window being resized
 */
void resize(int new_x, int new_y, int x_res[], int y_res[], int x_pos[], 
            int y_pos[], WINDOW* ws[], char cur_scramble[]) {
    int title_center = (new_x - 2 - strlen(title_string)) / 2;
    int mid_y;

    x_res[TITLE] = x_res[SCRAMBLE] = x_res[CONTROLS] = new_x;

    // Resize and move windows
    wresize(ws[TITLE], y_res[TITLE], x_res[TITLE]);
    title_center = (new_x-2-strlen(title_string))/2; // For center align
    wresize(ws[SCRAMBLE], y_res[SCRAMBLE], x_res[TITLE]);

    mid_y = new_y-y_res[TITLE]-y_res[SCRAMBLE]-y_res[CONTROLS];
    y_res[HISTORY] = y_res[TIMER] = y_res[STATS] = mid_y;
    x_res[TIMER] = new_x-x_res[HISTORY]-x_res[STATS];
    y_pos[CONTROLS] = new_y-y_res[CONTROLS];

    wresize(ws[HISTORY], y_res[HISTORY], x_res[HISTORY]);
    wresize(ws[TIMER], y_res[TIMER], x_res[TIMER]);
    wresize(ws[STATS], y_res[STATS], x_res[STATS]);
    mvwin(ws[STATS], y_pos[STATS], x_res[HISTORY]+x_res[TIMER]);
    wresize(ws[CONTROLS], y_res[CONTROLS], x_res[CONTROLS]);
    mvwin(ws[CONTROLS], y_pos[CONTROLS], x_pos[CONTROLS]);

    // Clear windows
    clear_windows();

    // Redraw title, scramble and controls
    mvwprintw(ws[TITLE], 1, title_center, title_string);
    mvwprintw(ws[SCRAMBLE], 1, 2, "Scramble: %s", cur_scramble);
	print_controls(ws[CONTROLS], y_res[CONTROLS], x_res[CONTROLS]);
}

/* 
 * Stop the timer
 */
int stop_timer(char cur_scramble[], WINDOW* scramble, WINDOW* history,
               Time_list time_data, F_Time_list recent[], int msec, int y) {
    Time t = create_time(cur_scramble, msec);
    append(time_data, t);
	for (int i = 0; i < PB_NUM; i++) {
		f_append(recent[i], create_time(cur_scramble, msec));
	}
    print_time_data(history, time_data, y);

    generate_scramble(cur_scramble);
    mvwprintw(scramble, 1, 2, "Scramble: %s", cur_scramble);
    return FALSE;
}

/* 
 * Take the starting time and return the number of milliseconds passed
 * Pre: start time is set
 * Post: msec is set
 */
int update_timer(struct timespec start) {
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    int ms = (end.tv_sec - start.tv_sec)*1000 // Seconds
             + (end.tv_nsec - start.tv_nsec)/1000000; // Nanoseconds
    return ms;
}
