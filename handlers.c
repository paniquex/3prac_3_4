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
deficit_money(int sig) {
    char *notEnoughMoneyStr = "Not enough money\n";
    int time = 3;
    fprintf(stdout, "%s", notEnoughMoneyStr);
    //fflush(stdout);
    //alarm(time);
    //pause();
    sleep(time);
    //fflush(stdout);
}


void
enough_money(int sig) { //need to rename this function
    char *amountStatus = "Current amount: ";
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
        //fflush(stdout);
        //alarm(time);
        //pause();
        screen_status = 1;
        sleep(time);
        screen_status = 0;
    }
    //fflush(stdout);
}


void
add_money(int sig) {
    char *amountStatus = "Current amount: ";
    amount += NOTE1;
    fprintf(stdout, "%s", amountStatus);
    fprintf(stdout, "%d\n", amount);
    //fflush(stdout);
}


void
status_screen(int sig) {
    printf("SCREEN: Current amount: %d\n", amount);
}

/* MECH handlers */
void
put_item(int sig) {
    fprintf(stdout, "Putting item away...\n");
    int time = 3;
    mech_status = 1;
    sleep(time);
    mech_status = 0;
    fprintf(stdout, "Get your item!\n");
}


void
status_mech(int sig) {
    if (mech_status == 1) {
        printf("MECH: Putting item away...\n");
    } else {
        printf("MECH: Idle...\n");
    }
}


/* NOTECHANGER handlers */
void
receive_money(int sig) {
    noteschanger_status = 1;
    sleep(3);
    noteschanger_status = 0;
    kill(SCREEN_PID, USERSIGNAL1);
}


void
status_noteschanger(int sig) {
    if (noteschanger_status == 1) {
        printf("NOTECHANGER: Receiving money...\n");
    } else {
        printf("NOTECHANGER: Idle...\n");
    }
}


/* BUTTON handlers */
void
click(int sig) {
    kill(SCREEN_PID, USERSIGNAL2);
}


void
status_button(int sig) {
    printf("BUTTON: Was clicked.\n");
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
    kill(SCREEN_PID, USERSIGNAL3);
    kill(MECH_PID, USERSIGNAL3);
    kill(NOTESCHANGER_PID, USERSIGNAL3);
    kill(BUTTON_PID, USERSIGNAL3);
    pause();
    printf("***************  STATUS  ***************\n");
}