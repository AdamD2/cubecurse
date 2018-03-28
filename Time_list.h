#include <string.h>

typedef struct _time_list* Time_list;
typedef struct _time*      Time;

// Create a new list with no values
Time_list new_list();

// Removes all memory associated with a list
void destroy(Time_list l);

// Create a node to be added to the list
Time create_time(char scramble[], int ms);
Time create_time_all(char scramble[], int ms, int plus_two, int dnf, char* comment);

// Append a node to the list
void append(Time_list l, Time t);

// Check if a specific solve is in the list
int  contains(Time_list l, Time t);

// Get the length of the list
int  tl_length(Time_list l);

// Get a node from a specific point in the list
Time get_time(Time_list l, int index);
// Get the first node of the list
Time get_head(Time_list l);
// Get the last node of the list
Time get_tail(Time_list l);

// Delete a node at a specific point in the list
Time delete_time(Time_list l, int index);

// Getter methods for a node
char* get_scramble(Time t);
int   get_ms(Time t);
int   get_plus_two(Time t);
int   get_dnf(Time t);
char* get_comment(Time t);

// Setter methods for a node
// Change plus two and dnf change the last node values between 0 and 1
void change_plus_two(Time_list l);
void change_dnf(Time_list l);
// At changes the value of plus two or dnf at a specific index
void change_plus_two_at(Time_list l, int index);
void change_dnf_at(Time_list l, int index);

// Print to the given window up to a given index
void print_up(WINDOW* w, Time_list l, int index);

// Print to the given window down to a given index
void print_down(WINDOW* w, Time_list l, int index);

// Update stats based on the most recent recorded time
// Assumes that there are enough times to test against for each category
void calculate_ao100(Time t, Time_list ao100);
void calculate_ao12(Time t, Time_list ao12);
void calculate_ao5(Time t, Time_list ao5);
void calculate_best(Time t, Time_list best);

// Update stats based on all recorded times, for example after a time is 
// deleted 
// Assumes that there are enough times to test against for each category
void calculate_ao100_all(Time_list l, Time_list ao100);
void calculate_ao12_all(Time_list l, Time_list ao12);
void calculate_ao5_all(Time_list l, Time_list ao5);
void calculate_best_all(Time_list l, Time_list best);


