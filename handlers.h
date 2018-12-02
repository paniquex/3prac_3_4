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

enum {NOTE1 = 25, NOTE2 = 60};
enum {USERSIGNAL1 = 30, USERSIGNAL2 = 10, USERSIGNAL3 = 16,
    USERSIGNAL4 = 31, USERSIGNAL5 = 12, USERSIGNAL6 = 17};

//char *screen_status_message[MAX_STATUS_MESSAGE_SIZE] = {"\0"};
//char *mech_status_message[MAX_STATUS_MESSAGE_SIZE] = {"\0"};
//char *noteschanger_status_message[MAX_STATUS_MESSAGE_SIZE] = {"\0"};
//char *button_status_message[MAX_STATUS_MESSAGE_SIZE] = {"\0"};
int screen_status, mech_status, noteschanger_status, button_status, button_ready_to_click;


pid_t SCREEN_PID, BUTTON_PID, NOTESCHANGER_PID, MECH_PID; // const but no? codestyle!!


/* SCREEN handlers */
void
enough_money(int sig);

void
add_money1(int sig);

void
add_money2(int sig);

void
status_screen(int sig);


/* MECH handlers: */
void
put_item(int sig);

void
status_mech(int sig);

/* NOTECHANGER handlers: */
void
receive_money1(int sig);

void
receive_money2(int sig);

void
status_noteschanger(int sig);

/* BUTTON handlers: */
void
click(int sig);

void
item_was_put_away(int sig);

void
status_button(int sig);

/*SIGTERM */
void
sigterm_handler(int sig);


/*SIGINT */
void
sigint_handler(int sig);


#endif //PRAC3_3_4_HANDLERS_H
