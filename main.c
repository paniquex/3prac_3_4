#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
//#include "handlers.h"


int amount = 30;
const int ITEM_COST = 20;

const char* SCREEN_SCRIPT_NAME = "screen_script.sh";
const char* MECH_SCRIPT_NAME = "mech_script.sh";
const char* NOTESCHANGER_SCRIPT_NAME = "noteschanger_script.sh";
const char* BUTTON_SCRIPT_NAME = "button_script.sh";

enum {NOTE1 = 25, NOTE2 = 60};
enum {USERSIGNAL1 = 30, USERSIGNAL2 = 10, USERSIGNAL3 = 16,
    USERSIGNAL4 = 31, USERSIGNAL5 = 12, USERSIGNAL6 = 17};

int screen_status, mech_status, noteschanger_status, button_status;

int button_ready_to_click, button_clicks;

pid_t SCREEN_PID, BUTTON_PID, NOTESCHANGER_PID, MECH_PID;


/* SCREEN handlers */
void
enough_money(int sig);

void
add_money(int sig);

void
status_screen(int sig);

void
alarm_screen(int sig);


/* MECH handlers: */
void
put_item(int sig);

void
status_mech(int sig);

/* NOTECHANGER handlers: */
void
receive_money(int sig);


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




int screen_flag = 1, mech_flag = 1, notechanger_flag = 1, button_flag = 1; // we can do it only one

int
script_writer(pid_t first_pid, pid_t second_pid, pid_t third_pid, pid_t fourth_pid);

int
script_creator(const char *script_name, pid_t pid, int signal_id);

int main() {
    fprintf(stdout, "%d\n", getpid());
    //init status messages;
    int fd[2];
    pipe(fd);

    if ((MECH_PID = fork()) == 0) {
        close(fd[1]);
        read(fd[0], &BUTTON_PID, sizeof(pid_t));
        close(fd[0]);
        printf("%d\n", BUTTON_PID);
        signal(USERSIGNAL1, put_item);
        signal(SIGINT, SIG_IGN); //ignore SIGINT
        signal(USERSIGNAL3, status_mech);
        do {
            pause();
            if (mech_status == 2) {
                fprintf(stdout, "Putting item away...\n");
                unsigned int time = 3;
                mech_status = 1;
                sleep(time);
                mech_status = 0;
                fprintf(stdout, "Get your item!\n");
                kill(BUTTON_PID, USERSIGNAL2);
            }
            mech_status = 0;
        } while (mech_flag == 1);
        return 0;
    }


    if ((SCREEN_PID = fork()) == 0) {
        screen_status = 0;
        close(fd[1]);
        read(fd[0], &BUTTON_PID, sizeof(pid_t));
        close(fd[0]);
        signal(SIGALRM, alarm_screen);
        //alarm(5);
        signal(USERSIGNAL2, enough_money);
        signal(USERSIGNAL1, add_money);
        signal(USERSIGNAL4, add_money);
        signal(SIGINT, SIG_IGN); //ignore SIGINT
        signal(USERSIGNAL3, status_screen);
        char *amountStatus = "Current amount: ";
        do {
            pause();
            if (screen_status == 2) { //enough_money
                //signal(USERSIGNAL2, SIG_IGN); ignore USERSIGNAL2 from button before item has put away
                if (amount >= ITEM_COST) {
                    amount -= ITEM_COST;
                    kill(MECH_PID, USERSIGNAL1);
                    fprintf(stdout, "%s", amountStatus);
                    fprintf(stdout, "%d\n", amount);
                } else {
                    unsigned int time = 3;
                    char *notEnoughMoneyStr = "Not enough money\n";
                    fprintf(stdout, "%s", notEnoughMoneyStr);
                    fprintf(stdout, "%s", amountStatus);
                    fprintf(stdout, "%d\n", amount);
                    kill(BUTTON_PID, USERSIGNAL2);
                    screen_status = 1;
                    sleep(time);
                    screen_status = 0;
                }
            }
            if (screen_status == 3) { //add_money
                amount += NOTE1;
                fprintf(stdout, "%s", amountStatus);
                fprintf(stdout, "%d\n", amount);
            }
            if (screen_status == 4) {
                amount += NOTE2;
                fprintf(stdout, "%s", amountStatus);
                fprintf(stdout, "%d\n", amount);
            }
            screen_status = 0;
        } while (screen_flag == 1);
        return 0;
    }


    if ((BUTTON_PID = fork()) == 0) {
        close(fd[0]);
        close(fd[1]);
        button_status = 0;
        button_ready_to_click = 1;
        signal(USERSIGNAL1, click);
        signal(USERSIGNAL2, item_was_put_away);
        signal(SIGINT, SIG_IGN); //ignore SIGINT
        signal(USERSIGNAL3, status_button);
        do {
            pause();
            if (button_status == 1) {
                if (button_ready_to_click == 1) {
                    kill(SCREEN_PID, USERSIGNAL2);
                    button_ready_to_click = 0;
                    pause(); //waiting for signal from MECH, that it has done work
                }
                button_ready_to_click = 1;
            }
            button_status = 0;
        } while (button_flag == 1);
        return 0;
    }
    write(fd[1], &BUTTON_PID, sizeof(pid_t));
    write(fd[1], &BUTTON_PID, sizeof(pid_t));
    close(fd[0]);
    close(fd[1]);


    if ((NOTESCHANGER_PID = fork()) == 0) {
        close(fd[0]);
        close(fd[1]);
        signal(USERSIGNAL1, receive_money);
        signal(USERSIGNAL2, receive_money);
        signal(SIGINT, SIG_IGN); //ignore SIGINT
        signal(USERSIGNAL3, status_noteschanger);
        do {
            pause();
            if (noteschanger_status == 2) {
                printf("Receiving %d coins\n", NOTE1);
                noteschanger_status = 1;
                signal(USERSIGNAL2, SIG_IGN);
                sleep(3);
                signal(USERSIGNAL2, receive_money);
                noteschanger_status = 0;
                kill(SCREEN_PID, USERSIGNAL1);
            }
            if (noteschanger_status == 3) {
                printf("Receiving %d coins\n", NOTE2);
                noteschanger_status = 1;
                signal(USERSIGNAL1, SIG_IGN);
                sleep(3);
                signal(USERSIGNAL1, receive_money);
                noteschanger_status = 0;
                kill(SCREEN_PID, USERSIGNAL4);
            }
            noteschanger_status = 0;
        } while (notechanger_flag == 1);
        return 0;
    }


    script_writer(SCREEN_PID, MECH_PID, NOTESCHANGER_PID, BUTTON_PID);
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}


int
script_writer(pid_t first_pid, pid_t second_pid, pid_t third_pid, pid_t fourth_pid) {
    script_creator(SCREEN_SCRIPT_NAME, first_pid, USERSIGNAL1);
    script_creator(MECH_SCRIPT_NAME, second_pid ,USERSIGNAL1);
    script_creator(NOTESCHANGER_SCRIPT_NAME, third_pid, USERSIGNAL1);
    script_creator(BUTTON_SCRIPT_NAME, fourth_pid, USERSIGNAL1);
    return 0;
}

int
script_creator(const char *script_name, pid_t pid, int signal_id) {
    FILE *file = fopen(script_name, "w+");
    char *start_str = "#!/bin/bash\nkill ";
    fprintf(file, "%s-%d ", start_str, signal_id);
    fprintf(file, "%d\n", pid);
    fclose(file);
    return 0;
}


/* SCREEN handlers */
void
enough_money(int sig) { //need to rename this function
    screen_status = 2;
}


void
add_money(int sig) {
    if (sig == USERSIGNAL1) {
        screen_status = 3;
    } else if (sig == USERSIGNAL4) {
        screen_status = 4;
    }
}


void
status_screen(int sig) {
    fprintf(stderr, "SCREEN: Current amount: %d\n", amount);
}


void
alarm_screen(int sig) {
    printf("Current amount: %d\n", amount);
    alarm(5);
}

/* MECH handlers */
void
put_item(int sig) {
    mech_status = 2;
}


void
status_mech(int sig) {
    if (mech_status == 1) {
        fprintf(stderr, "MECH: Putting item away...\n");
    } else {
        fprintf(stderr, "MECH: Idle...\n");
    }
}


/* NOTECHANGER handlers */
void
receive_money(int sig) {
    if (noteschanger_status == 0) {
        if (sig == USERSIGNAL1) {
              noteschanger_status = 2;
        } else if (sig == USERSIGNAL2) {
            noteschanger_status = 3;
        }
    }
}


void
status_noteschanger(int sig) {
    if (noteschanger_status == 1) {
        fprintf(stderr, "NOTECHANGER: Receiving money...\n");
    } else {
        fprintf(stderr, "NOTECHANGER: Idle...\n");
    }
}


/* BUTTON handlers */
void
click(int sig) {
    button_clicks++;
    button_status = 1;
}

void
item_was_put_away(int sig) {
    //just for remove pause from BUTTON
}

void
status_button(int sig) {
    fprintf(stderr, "BUTTON: was clicked %d times.\n", button_clicks);
}


/* SIGTERM */
void
sigterm_handler(int sig) {
    kill(SCREEN_PID, SIGTERM);
    kill(MECH_PID, SIGTERM);
    kill(NOTESCHANGER_PID, SIGTERM);
    kill(BUTTON_PID, SIGTERM);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    unlink(SCREEN_SCRIPT_NAME);
    unlink(MECH_SCRIPT_NAME);
    unlink(NOTESCHANGER_SCRIPT_NAME);
    unlink(BUTTON_SCRIPT_NAME);
}


/*SIGINT */
void
sigint_handler(int sig) {
    printf("***************  STATUS  ***************\n");
    sleep(1);
    kill(SCREEN_PID, USERSIGNAL3);
    sleep(1);
    kill(MECH_PID, USERSIGNAL3);
    sleep(1);
    kill(NOTESCHANGER_PID, USERSIGNAL3);
    sleep(1);
    kill(BUTTON_PID, USERSIGNAL3);
    sleep(1);
    printf("***************  STATUS  ***************\n");
    //pause();
}