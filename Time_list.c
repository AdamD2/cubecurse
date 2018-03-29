// Implements all functions declared in list.h

#include <stdlib.h>
#include <curses.h>
#include <assert.h>
#include "Time_list.h"

// Local functions
void copy_time(Time source, Time dest);

// Structs
typedef struct _time_list {
    Time head;
    Time tail;
    int  length;
} time_list;

typedef struct _time {
    char  scramble[60];
    int   ms;
    int   plus_two;
    int   dnf;
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

void destroy(Time_list l) {
    Time cur = l->head;
    Time prev = NULL;

    while (cur != NULL) {
        prev = cur;
        cur = prev->next;
        free(prev);
    }

    free(l);
}

Time create_time(char scramble[], int ms) {
    Time t = malloc(sizeof(time));
    assert(t != NULL);

    memcpy((void *)t->scramble, (void *)scramble, 60);
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

    memcpy((void *)t->scramble, (void *)scramble, 60);
    t->ms = ms;
    t->plus_two = plus_two;
    t->dnf = dnf;
    if (comment != NULL) {
        memcpy((void *)t->comment, (void *)comment, 100);
    } else {
        t->comment = NULL;
    }
    t->prev = t->next = NULL;

    return t;
}

void append(Time_list l, Time t) {
    if (l->tail == NULL) {
        l->head = t;
        l->tail = t;
    } else if (l->head == l->tail) {
        l->tail = t;
        l->head->next = t;
        t->prev = l->head;
    } else {
        l->tail->next = t;
        t->prev = l->tail;
        l->tail = t;
    }

    l->length++;
}

int contains(Time_list l, Time t) {
    Time cur = l->head;
    while (cur != NULL) {
        // Only need to check the scramble, since it is almost certainly unique
        if (strcmp(t->scramble, cur->scramble) == 0) {
            return TRUE;
        }

        cur = cur->next;
    }
}

int tl_length(Time_list l) {
    return l->length;
}

Time get_time(Time_list l, int index) {
    // TODO
    return NULL;
}

Time get_head(Time_list l) {
    return l->head;
}

Time get_tail(Time_list l) {
    return l->tail;
}

Time delete_time(Time_list l, int index) {
    // TODO
    return NULL;
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
    if (l->tail->plus_two == 0) {
        l->tail->plus_two = 1;
    } else {
        l->tail->plus_two = 0;
    }
}

void change_dnf(Time_list l) {
    if (l->tail->dnf == 0) {
        l->tail->dnf = 1;
    } else {
        l->tail->dnf = 0;
    }
}

void change_plus_two_at(Time_list l, int index) {
    assert(index <= l->length);
    Time cur = l->head;

    for (int y = 1; y <= index; y++) {
        cur = cur->next;
    }
    
    if (cur->plus_two == 0) {
        cur->plus_two = 1;
    } else {
        cur->plus_two = 0;
    }
}

void change_dnf_at(Time_list l, int index) {
    assert(index <= l->length);
    Time cur = l->head;

    for (int y = 1; y <= index; y++) {
        cur = cur->next;
    }
    
    if (cur->dnf == 0) {
        cur->dnf = 1;
    } else {
        cur->dnf = 0;
    }
}

void print_up(WINDOW* w, Time_list l, int num) {
    assert(num <= l->length);
    Time cur = l->head;
    float time;

    for (int y = 1; y <= num; y++) {
        time = ((float)(cur->ms))/1000;

        if (cur->dnf) {
            mvwprintw(w, y, 2, "DNF  ");
        } else if (cur->plus_two) {
            mvwprintw(w, y, 2, "%.2f+", time+2);
        } else {
            mvwprintw(w, y, 2, "%.2f ", time);
        }

        cur = cur->next;
    }
}

void print_down(WINDOW* w, Time_list l, int num) {
    assert(num <= l->length);
    Time cur = l->tail;
    float time;

    for (int y = num; y > 0; y--) {
        time = ((float)(cur->ms))/1000;

        if (cur->dnf) {
            mvwprintw(w, y, 2, "DNF  ");
        } else if (cur->plus_two) {
            mvwprintw(w, y, 2, "%.2f+", time+2);
        } else {
            mvwprintw(w, y, 2, "%.2f", time);
        }

        cur = cur->prev;
    }
}

void calculate_ao100(Time t, Time_list ao100) {
}

void calculate_ao12(Time t, Time_list ao12) {
}

void calculate_ao5(Time t, Time_list ao5) {
    Time new;

    if (ao5->length == 0) {
        for (int i = 0; i < 5; i++) {
            new = create_time_all(t->scramble, t->ms, t->plus_two, t->dnf,
                                  t->comment);
            append(ao5, new);
            t = t->prev;
        }
    } else {
        // TODO calculate ao5 for original and new and compare
    }
}

void calculate_best(Time t, Time_list best) {
    if (best->length == 0) {
        Time new = create_time_all(t->scramble, t->ms, t->plus_two, 
                                   t->dnf, t->comment);
        append(best, new);
    } else {
        Time old = best->head;

        if (old->dnf == 1 && t->dnf == 0) {
            copy_time(t, old);
        } else if (t->dnf == 0 && t->ms+2000*t->plus_two < 
                   old->ms+2*old->plus_two) {
            copy_time(t, old);
        }
    }
}

void calculate_ao100_all(Time_list l, Time_list ao100){
    Time cur = l->head;
    for (int i = 0; i < 99; i++) {
        cur = cur->next;
    }

    while (cur != NULL) {
        calculate_ao100(cur, ao100);
        cur = cur->next;
    }
}

void calculate_ao12_all(Time_list l, Time_list ao12){
    Time cur = l->head;
    for (int i = 0; i < 11; i++) {
        cur = cur->next;
    }

    while (cur != NULL) {
        calculate_ao12(cur, ao12);
        cur = cur->next;
    }
}

void calculate_ao5_all(Time_list l, Time_list ao5){
    Time cur = l->head;
    for (int i = 0; i < 4; i++) {
        cur = cur->next;
    }

    while (cur != NULL) {
        calculate_ao5(cur, ao5);
        cur = cur->next;
    }
}

void calculate_best_all(Time_list l, Time_list best) {
    Time cur = l->head;

    //delete_time(l, 0);
    // Workaround until delete is written
    destroy(best);
    best = new_list();
    while (cur != NULL) {
        calculate_best(cur, best);
        cur = cur->next;
    }
}

/*
 * Copy a single time from one node to another
 */
void copy_time(Time source, Time dest) {
    memcpy((void*)dest->scramble, (void*)source->scramble, 60);
    dest->ms = source->ms;
    dest->plus_two = source->plus_two;
    dest->dnf = source->dnf;
    if (source->comment != NULL) {
        memcpy((void*)dest->comment, (void*)source->comment, 100);
    } else {
        free(dest->comment);
        dest->comment = NULL;
    }
}
