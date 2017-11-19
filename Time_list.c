// Implements all functions declared in list.h

#include <stdlib.h>
#include <curses.h>
#include <assert.h>
#include "Time_list.h"

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

Time create_time_all(char scramble[], int ms, int plus_two, int dnf, char* comment) {
    Time t = malloc(sizeof(time));
    assert(t != NULL);

    memcpy((void *)t->scramble, (void *)scramble, 60);
    t->ms = ms;
    t->plus_two = plus_two;
    t->dnf = dnf;
    t->comment = comment;
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

int tl_length(Time_list l) {
    return l->length;
}

Time get_time(Time_list l, int index) {
    // TODO
    return NULL;
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
    // TODO implement
}

void change_dnf_at(Time_list l, int index) {
    // TODO implement
}

void print_up(WINDOW* w, Time_list l, int num) {
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
    Time cur = l->tail;
    for (int y = num; y > 0; y--) {
        if (cur->dnf) {
            mvwprintw(w, y, 1, "DNF");
        } else if (cur->plus_two) {
            mvwprintw(w, y, 2, "%d.%d+", (cur->ms/1000)+2, cur->ms%1000);
        } else {
            mvwprintw(w, y, 2, "%d.%d", cur->ms/1000, cur->ms%1000);
        }

        cur = cur->prev;
    }
}
