/*
 * Cubecurse is a simple ncurses based timer developed for timing Rubik's Cube
 * speedsolves.
 *
 * Author: Adam Douglas <adam_douglas2@hotmail.com>
 * Github: https://github.com/adamd2/cubecurse
 */

#include "cubecurse.h"

int main(int argc, char* argv[]) {
    // Printing variables
    char current_scramble[60];

    initscr();      // Initialize curses
    keypad(stdscr, TRUE);   // Enable keyboard mapping
    nonl();         // No NL->CR/NL on output
    cbreak();       // Take input one at a time
    nodelay(stdscr, TRUE);  // Do not pause on getch()
    curs_set(0);    // Make cursor invisible
    echo();         // Do not echo input
    clock_t t = clock();
    srand((unsigned) time(&t)); // Seed rand

    // Set up some basic colors
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED,     BACKGROUND);
        init_pair(2, COLOR_GREEN,   BACKGROUND);
        init_pair(3, COLOR_BLUE,    BACKGROUND);
        init_pair(4, COLOR_BLACK,   COLOR_GREEN);
    }

    // Create window based variables
    int parent_x, parent_y, new_x, new_y;
    getmaxyx(stdscr, parent_y, parent_x);   // Get max dimensions
    int title_spacing = (parent_x - 2 - strlen(title_string)) / 2; // Center align
    int middle_y = parent_y - 3 - 3 - 4;

    int y_res[6] = {3, 3, middle_y, middle_y, middle_y, 4};
    int x_res[6] = {parent_x, parent_x, 15, 
                    parent_x-15-29, 29, parent_x};

    int mid_pos = y_res[TITLE]+y_res[SCRAMBLE];

    int y_pos[6] = {0, y_res[SCRAMBLE], mid_pos, mid_pos, mid_pos, 
                    parent_y-y_res[CONTROLS]};
    int x_pos[6] = {0, 0, 0, x_res[HISTORY], parent_x-x_res[STATS], 0};

    // Window setup
    WINDOW *title = newwin(y_res[TITLE], x_res[TITLE], y_pos[TITLE], x_pos[SCRAMBLE]);
    WINDOW *scramble = newwin(y_res[SCRAMBLE], x_res[SCRAMBLE], y_res[SCRAMBLE], x_pos[SCRAMBLE]);
    WINDOW *history = newwin(y_res[HISTORY], x_res[HISTORY], y_pos[HISTORY], x_pos[HISTORY]);
    WINDOW *timer = newwin(y_res[TIMER], x_res[TIMER], y_pos[TIMER], x_pos[TIMER]);
    WINDOW *stats = newwin(y_res[STATS], x_res[STATS], y_pos[STATS], x_pos[STATS]);
    WINDOW *controls = newwin(y_res[CONTROLS], x_res[CONTROLS], y_pos[CONTROLS], x_pos[CONTROLS]);

    // Time data setup
    Time_list time_data = time_data_setup();
    Time_list pbs[4];
    pb_setup(pbs);

    // Initial printing
    add_boxes();
    mvwprintw(title, 1, title_spacing, title_string);
    generate_scramble(current_scramble);
    mvwprintw(scramble, 1, 2, "Scramble: %s", current_scramble);
    mvwprintw(controls, 1, 2, "%s", controls_string);
    print_time_data(history, time_data, y_res[HISTORY]);
    refresh_windows();

    // Main loop
    main_loop(x_res, y_res, x_pos, y_pos, title, scramble, history, timer,
              stats, controls, time_data, current_scramble);

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
    } else if (l <= y) {
        print_up(history, time_data, tl_length(time_data));
    } else {
        print_down(history, time_data, y-2);
    }
}

/*
 * Load in previous pbs or create empty lists for them
 */
void pb_setup(Time_list pbs[]) {
    pbs[BEST] = new_list();
    pbs[AO5] = new_list();
    pbs[AO12] = new_list();
    pbs[AO100] = new_list();

    // TODO load from file
}

/*
 * Calculate stats and display them on the right window
 */
void calculate_stats(WINDOW* stats, Time_list time_data, Time_list pbs[]) {
    int len = tl_length(time_data);
    if (len >= 1) {
        calculate_best(time_data, pbs[BEST]);
        if (len >= 5) {
            calculate_ao5(time_data, pbs[AO5]);
            if (len >= 12) {
                calculate_ao12(time_data, pbs[AO12]);
                if (len >= 100) {
                    calculate_ao100(time_data, pbs[AO100]);
                }
            }
        }
    }
}

/*
 * Take an array of chars and put in a 3x3x3 scramble
 * Pre: current_scramble is 60 chars long
 * Post: current_scramble contains a 20 move scramble
 */
void generate_scramble(char current_scramble[]) {
    int index = 0;
    int rand_num = 0, prev_rand;
    for (int i = 0; i < 20; i++) {
        prev_rand = rand_num;
        rand_num = rand() % 18;
        while (rand_num-(rand_num%3) == prev_rand-(prev_rand%3)) {
            rand_num = rand() % 18;
        }
        strcpy(&current_scramble[index], scramble_options[rand_num]);
        index += strlen(scramble_options[rand_num]);
    }
    for (index; index < 60; index++) {
        strcpy(&current_scramble[index], " ");
    }
}

/* 
 * Main loop of the program
 */
void main_loop(int x_res[], int y_res[], int x_pos[], int y_pos[],
               WINDOW* title, WINDOW* scramble, WINDOW* history, WINDOW* timer,
               WINDOW* stats, WINDOW* controls, Time_list time_data,
               char current_scramble[]) {
    // Resizing variables
    int parent_y, parent_x, new_y, new_x;
    getmaxyx(stdscr, parent_y, parent_x);

    // Timing variables
    int timing = FALSE;
    clock_t start;
    int msec = 0;

    while (TRUE) {
        int c = getch();

        // Handle resizing
        getmaxyx(stdscr, new_y, new_x);
        if (new_y != parent_y || new_x != parent_x) {
            parent_y = new_y;
            parent_x = new_x;
            resize(new_x, new_y, x_res, y_res, x_pos, y_pos, title, scramble,
                   history, timer, stats, controls, current_scramble);
        }

        // Handle keyboard input
        if (c == 'n') {
            generate_scramble(current_scramble);
        } else if (c == 'q') {
            break;
        } else if (c == ' ' && timing == FALSE) {
            timing = TRUE;
            start = clock();
        } else if (c == ' ' && timing == TRUE) {
            timing = stop_timer(current_scramble, scramble, history, time_data,
                                msec, y_res[HISTORY]);
        } else if (c == 'f') {
            change_dnf(time_data);
            print_time_data(history, time_data, y_res[HISTORY]);
        } else if (c == '+') {
            change_plus_two(time_data);
            print_time_data(history, time_data, y_res[HISTORY]);
        }

        if (timing) msec = update_timer(start);

        mvwprintw(timer, y_res[TIMER]/2, x_res[TIMER]/2, "%d.%d", msec/1000, msec%1000);

        // Refresh each window
        add_boxes();
        refresh_windows();
    }
}

/*
 * Handle the event of the window being resized
 */
void resize(int new_x, int new_y, int x_res[], int y_res[], int x_pos[], 
            int y_pos[], WINDOW* title, WINDOW* scramble, WINDOW* history,
            WINDOW* timer, WINDOW* stats, WINDOW* controls, char cur_scram[]) {
    int title_spacing = (new_x - 2 - strlen(title_string)) / 2;
    int middle_y;

    x_res[TITLE] = x_res[SCRAMBLE] = x_res[CONTROLS] = new_x;

    // Resize and move windows
    wresize(title, y_res[TITLE], x_res[TITLE]);
    title_spacing = (new_x-2-strlen(title_string))/2; // For center align
    wresize(scramble, y_res[SCRAMBLE], x_res[TITLE]);

    middle_y = new_y-y_res[TITLE]-y_res[SCRAMBLE]-y_res[CONTROLS];
    y_res[HISTORY] = y_res[TIMER] = y_res[STATS] = middle_y;
    x_res[TIMER] = new_x-x_res[HISTORY]-x_res[STATS];
    y_pos[CONTROLS] = new_y-y_res[CONTROLS];

    wresize(history, y_res[HISTORY], x_res[HISTORY]);
    wresize(timer, y_res[TIMER], x_res[TIMER]);
    wresize(stats, y_res[STATS], x_res[STATS]);
    mvwin(stats, y_pos[STATS], x_res[HISTORY]+x_res[TIMER]);
    wresize(controls, y_res[CONTROLS], x_res[CONTROLS]);
    mvwin(controls, y_pos[CONTROLS], x_pos[CONTROLS]);

    // Clear windows
    clear_windows();

    // Redraw title, scramble and controls
    mvwprintw(title, 1, title_spacing, title_string);
    mvwprintw(scramble, 1, 2, "Scramble: %s", cur_scram);
    mvwprintw(controls, 1, 2, "%s", controls_string);
}

/* 
 * Stop the timer
 */
int stop_timer(char current_scramble[], WINDOW* scramble, WINDOW* history,
               Time_list time_data, int msec, int y) {
    generate_scramble(current_scramble);
    mvwprintw(scramble, 1, 2, "Scramble: %s", current_scramble);
    Time t = create_time(current_scramble, msec);
    append(time_data, t);
    print_time_data(history, time_data, y);
    return FALSE;
}

/* 
 * Take the starting time and return the number of milliseconds passed
 * Pre: start time is set
 * Post: msec is set
 */
int update_timer(clock_t start) {
    return (clock() - start)*1000/CLOCKS_PER_SEC;
}
