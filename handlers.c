#include "handlers.h"

int amount = 35;
const int ITEM_COST = 20;


const char* SCREEN_SCRIPT_NAME = "screen_script.sh";
const char* MECH_SCRIPT_NAME = "mech_script.sh";
const char* NOTESCHANGER_SCRIPT_NAME = "noteschanger_script.sh";
const char* BUTTON_SCRIPT_NAME = "button_script.sh";
int screen_status = 0, mech_status = 0, noteschanger_status = 0, button_status = 0;

/* SCREEN handlers */
void
enough_money(int sig) { //need to rename this function
    char *amountStatus = "Current amount: ";
    //signal(USERSIGNAL2, SIG_IGN); //ignore USERSIGNAL2 from button before item has put away
    if (amount >= ITEM_COST) {
        amount -= ITEM_COST;
        kill(MECH_PID, USERSIGNAL1);
        fprintf(stdout, "%s", amountStatus);
        fprintf(stdout, "%d\n", amount);
    } else {
        int time = 3;
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


void
add_money(int sig) {
    char *amountStatus = "Current amount: ";
    if (sig == USERSIGNAL1) {
        amount += NOTE1;
    } else if (sig == USERSIGNAL4) {
        amount += NOTE2;
    }
    fprintf(stdout, "%s", amountStatus);
    fprintf(stdout, "%d\n", amount);
}


void
status_screen(int sig) {
    fprintf(stderr, "SCREEN: Current amount: %d\n", amount);
}

/* MECH handlers */
void
put_item(int sig) {
    fprintf(stdout, "Putting item away...\n");
    unsigned int time = 3;
    mech_status = 1;
    sleep(time);
    mech_status = 0;
    fprintf(stdout, "Get your item!\n");
    kill(BUTTON_PID, USERSIGNAL2);
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
            noteschanger_status = 1;
            signal(USERSIGNAL2, SIG_IGN);
            sleep(3);
            signal(USERSIGNAL2, receive_money);
            noteschanger_status = 0;
            kill(SCREEN_PID, USERSIGNAL1);
        } else if (sig == USERSIGNAL2) {
            noteschanger_status = 1;
            signal(USERSIGNAL1, SIG_IGN);
            sleep(3);
            signal(USERSIGNAL1, receive_money);
            noteschanger_status = 0;
            kill(SCREEN_PID, USERSIGNAL4);
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
    button_status++;
    if (button_ready_to_click == 1) {
        kill(SCREEN_PID, USERSIGNAL2);
        button_ready_to_click = 0;
        pause(); //waiting for signal from MECH, that it has done work
    }
    button_ready_to_click = 1;
}

void
item_was_put_away(int sig) {
    //just for remove pause from BUTTON
}

void
status_button(int sig) {
    fprintf(stderr, "BUTTON: was clicked %d times.\n", button_status);
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