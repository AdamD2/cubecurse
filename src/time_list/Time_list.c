// Implements all functions declared in Time_list.h

#include <stdlib.h>
#include <curses.h>
#include <assert.h>
#include "Time_list.h"

// Local functions
void f_update_fast(F_Time_list l);
void f_update_slow(F_Time_list l);
Time clone_time(Time source);
void f_copy_list(F_Time_list source, F_Time_list dest);

// Structs
typedef struct _time_list {
    Time head;
    Time tail;
    unsigned int  length;
} time_list;

typedef struct _f_time_list {
	// Fixed time list
	Time head;
	Time tail;
	Time fast;	// Fastest time
	Time slow;  // Slowest time
	unsigned int length; 
	unsigned int max;	 // Max number of times
	unsigned int sum;	 // Sum of times
} f_time_list;

typedef struct _time {
    char  scramble[SCRAMBLE_LENGTH];
    unsigned int   ms;
    unsigned int   plus_two;
    unsigned int   dnf;
    char* comment;
    Time  prev;
    Time  next;
} time;

Time_list new_list() {
    Time_list l = malloc(sizeof(time_list));
    assert(l != NULL);

    l->head = NULL;
	l->tail = NULL;
    l->length = 0;

    return l;
}

F_Time_list new_f_list(int max) {
	F_Time_list l = malloc(sizeof(f_time_list));
	assert(l != NULL);

	l->head = l->tail = NULL;
	l->fast = l->slow = NULL;
	l->length = 0;
	l->max = max;
	l->sum = 0;
}

void destroy(Time_list l) {
    Time cur = l->head;
    Time prev = NULL;

    while (cur != NULL) {
        prev = cur;
        cur = prev->next;
        destroy_time(prev);
    }

    free(l);
}

void f_destroy(F_Time_list l) {
    Time cur = l->head;
    Time prev = NULL;

    while (cur != NULL) {
        prev = cur;
        cur = prev->next;
        destroy_time(prev);
    }

    free(l);
}

Time create_time(char scramble[], int ms) {
    Time t = malloc(sizeof(time));
    assert(t != NULL);

    memcpy((void *)t->scramble, (void *)scramble, SCRAMBLE_LENGTH);
    t->ms = ms;
    t->plus_two = 0;
    t->dnf = 0;
    t->comment = NULL;
    t->prev = t->next = NULL;

    return t;
}

Time create_time_all(char scramble[], int ms, int plus_two, int dnf, 
                     char* comment) {
    Time t = malloc(sizeof(time));
    assert(t != NULL);

    memcpy((void *)t->scramble, (void *)scramble, SCRAMBLE_LENGTH);
    t->ms = ms;
    t->plus_two = plus_two;
    t->dnf = dnf;
    if (comment != NULL) {
        memcpy((void *)t->comment, (void *)comment, COMMENT_LENGTH);
    } else {
        t->comment = NULL;
    }
    t->prev = t->next = NULL;

    return t;
}

void destroy_time(Time t) {
	if (t->comment != NULL) {
		free(t->comment);
	}
	free(t);
}

void append(Time_list l, Time t) {
    if (l->length == 0) {
        l->head = t;
        l->tail = t;
    } else {
        l->tail->next = t;
        t->prev = l->tail;
        l->tail = t;
    }

    l->length++;
}

void f_append(F_Time_list l, Time t) {
	if (l->length == 0) {
		l->head = t;
		l->tail = t;

		l->fast = t;
		l->slow = t;
		l->sum = t->ms;

		l->length++;
	} else if (l->length < l->max) {
		l->tail = t;
		l->head->next = t;
		t->prev = l->head;
	
		if (t->ms < l->fast->ms) {
			l->fast = t;
		} else if (t->ms > l->slow->ms) {
			l->slow = t;
		}
		l->sum += t->ms;

		l->length++;
	} else if (l->max == 1) {
		Time old = l->head;

		l->head = t;
		l->tail = t;

		l->fast = t;
		l->slow = t;
		l->sum = t->ms;

		destroy_time(old);
	} else if (l->length == l->max) {
		// Add the new node on the tail
		l->tail->next = t;
		t->prev = l->tail;
		l->tail = t;
		l->sum += t->ms;

		// Remove the old node from the head
		Time old = l->head;
		l->sum -= old->ms;
		l->head = old->next;
		old->next->prev = NULL;
	
		// Recalculate fast and slow
		if (l->fast == old) {
			f_update_fast(l);
		} else if (l->slow == old) {
			f_update_slow(l);
		} else {
			if (t->ms < l->fast->ms) {
				l->fast = t;
			} else if (t->ms > l->slow->ms) {
				l->slow = t;
			}
		}	

		destroy_time(old);
	}
}

void f_update_fast(F_Time_list l) {
	Time cur = l->head;
	l->fast->ms = INFINITY;
	while (cur != NULL) {
		if (cur->ms < l->fast->ms) {
			l->fast = cur;
		}
		cur = cur->next;
	}
}

void f_update_slow(F_Time_list l) {
	Time cur = l->head;
	l->slow->ms = 0;
	while (cur != NULL) {
		if (cur->ms > l->fast->ms) {
			l->slow = cur;
		}
		cur = cur->next;
	}
}

int contains(Time_list l, Time t) {
    Time cur = l->head;
    while (cur != NULL) {
        // Only need to check the scramble, since it is almost certainly unique
        if (strncmp(t->scramble, cur->scramble, SCRAMBLE_LENGTH) == 0) {
            return TRUE;
        }

        cur = cur->next;
    }

	return FALSE;
}

int tl_length(Time_list l) {
    return l->length;
}

Time get_time(Time_list l, int index) {
    assert(index < l->length);
    Time cur = l->head;

    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }

	return cur;
}

Time get_head(Time_list l) {
    return l->head;
}

Time get_tail(Time_list l) {
    return l->tail;
}

Time delete_time(Time_list l, int index) {
    assert(index < l->length);
    Time cur = l->head;

    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
 
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	destroy_time(cur);
	l->length--;
}

char* get_scramble(Time t) {
    return t->scramble;
}
int  get_ms(Time t) {
    return t->ms;
}
int  get_plus_two(Time t) {
    return t->plus_two;
}
int  get_dnf(Time t) {
    return t->dnf;
}
char* get_comment(Time t) {
    return t->comment;
}

void change_plus_two(Time_list l) {
	l->tail->plus_two = !l->tail->plus_two;
	l->tail->ms += 2000;
}

void change_dnf(Time_list l) {
	l->tail->dnf = !l->tail->dnf;
}

void change_plus_two_at(Time_list l, int index) {
    assert(index < l->length);
    Time cur = l->head;

    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    
	cur->plus_two = !cur->plus_two;
	cur->ms += 2000;
}

void change_dnf_at(Time_list l, int index) {
    assert(index < l->length);
    Time cur = l->head;

    for (int i = 0; i <= index; i++) {
        cur = cur->next;
    }
    
	cur->dnf = !cur->dnf;
}

void print_up(WINDOW* w, Time_list l) {
    Time cur = l->head;
    float time;

    for (int y = 1; y <= l->length; y++) {
        time = ((float)(cur->ms))/1000;

        if (cur->dnf) {
            mvwprintw(w, y, 2, "DNF  ");
        } else if (cur->plus_two) {
            mvwprintw(w, y, 2, "%.2f+", time);
        } else {
            mvwprintw(w, y, 2, "%.2f ", time);
        }

        cur = cur->next;
    }
}

void print_up_stats(WINDOW* w, F_Time_list l, char* label, int y) {
	if (f_average(l) < INFINITY) {
		mvwprintw(w, y, 2, "%s %.2f ", label, f_average(l)/1000);
	}
}

void print_down(WINDOW* w, Time_list l, int num) {
    assert(num < l->length);
    Time cur = l->tail;
    float time;

    for (int y = num; y > 0; y--) {
        time = ((float)(cur->ms))/1000;

        if (cur->dnf) {
            mvwprintw(w, y, 2, "DNF  ");
        } else if (cur->plus_two) {
            mvwprintw(w, y, 2, "%.2f+", time);
        } else {
            mvwprintw(w, y, 2, "%.2f", time);
        }

        cur = cur->prev;
    }
}

float f_average(F_Time_list l) {
	if (l->length != l->max) {
		return INFINITY;
	} else if (l->max < 4) {
		return (float)(l->sum)/l->max;
	} else {
		return (float)(l->sum - l->fast->ms - l->slow->ms)/l->max;
	}
}

void calculate_average(F_Time_list recent, F_Time_list best) {
	if (f_average(recent) < f_average(best)) {
		Time cur = recent->head;
		while (cur != NULL) {
			f_append(best, clone_time(cur));
			cur = cur->next;
		}
	}
}

void calculate_average_all(Time_list l, F_Time_list best) {
	// TODO
}

/*
 * Return a copy of the time passed in
 */
Time clone_time(Time source) {
	Time dest = create_time("", 0);

    memcpy((void*)dest->scramble, (void*)source->scramble, SCRAMBLE_LENGTH);
    dest->ms = source->ms;
    dest->plus_two = source->plus_two;
    dest->dnf = source->dnf;
    if (source->comment != NULL) {
        memcpy((void*)dest->comment, (void*)source->comment, COMMENT_LENGTH);
    } else {
		if (dest->comment != NULL) {
			free(dest->comment);
			dest->comment = NULL;
		}
    }

	return dest;
}

/*
 * Copy a fixed list
 */
/*void f_copy_list(F_Time_list source, F_Time_list dest) {
	Time st = source->head;
	Time dt = dest->head;

	for (int i = 0; i < source->max; i++) {
		copy_time(st, dt);
		st = st->next;
		dt = dt->next;
	}
}*/
