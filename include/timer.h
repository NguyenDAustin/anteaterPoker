#ifndef TIMER_H
#define TIMER_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *label;
    int time;
} Timer;

void start_timer(Timer *timer);

gboolean update_timer(gpointer data);

#endif