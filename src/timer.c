#include <stdio.h>
#include "timer.h"

gboolean update_timer(gpointer data)
{
    Timer *timer = (Timer *)data;

    timer->time++; //increment up

    char text[64]; //set character amount

    sprintf(text, "Time: %d", timer->time); //sprint to print the variable

    gtk_label_set_text(GTK_LABEL(timer->label), text);

    return TRUE;
}

void start_timer(Timer *timer)
{
    timer->time = 0; //

    gtk_label_set_text(GTK_LABEL(timer->label), "Time: 0"); //initialize

    g_timeout_add_seconds(1, update_timer, timer); //increment up
}


void reset_timer(Timer *timer)
{
    timer->time = 0;

    gtk_label_set_text(
        GTK_LABEL(timer->label),
        "Time: 0"
    );
}