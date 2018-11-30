#ifndef PRAC3_3_4_HANDLERS_H
#define PRAC3_3_4_HANDLERS_H
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

const char* SCREEN_SCRIPT_NAME;
const char* MECH_SCRIPT_NAME;
const char* NOTESCHANGER_SCRIPT_NAME;
const char* BUTTON_SCRIPT_NAME;

enum {NOTE1 = 25, NOTE2 = 50};
enum {USERSIGNAL1 = 30, USERSIGNAL2 = 10, USERSIGNAL3 = 16,
    USERSIGNAL4 = 31, USERSIGNAL5 = 12, USERSIGNAL6 = 17};

pid_t SCREEN_PID, BUTTON_PID, NOTESCHANGER_PID, MECH_PID; // const but no? codestyle!!


/* SCREEN handlers */
void
deficit_money(int sig);

void
enough_money(int sig);

void
add_money(int sig);


/* MECH handlers: */
void put_item(int sig);


/* NOTECHANGER handlers: */
void
receive_money(int sig);


/* BUTTON handlers: */
void
click(int sig);


/*SIGTERM */
void
sigterm_handler(int sig);


/*SIGINT */
void
sigint_handler(int sig);


#endif //PRAC3_3_4_HANDLERS_H
