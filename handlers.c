#include "handlers.h"

int amount = 35;
const int ITEM_COST = 20;


const char* SCREEN_SCRIPT_NAME = "screen_script.sh";
const char* MECH_SCRIPT_NAME = "mech_script.sh";
const char* NOTESCHANGER_SCRIPT_NAME = "noteschanger_script.sh";
const char* BUTTON_SCRIPT_NAME = "button_script.sh";



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
enough_money(int sig) {
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
        sleep(time);
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


/* MECH handlers */
void
put_item(int sig) {
    fprintf(stdout, "Putting item away...\n");
    int time = 3;
    sleep(time);
    fprintf(stdout, "Get your item!\n");
}


/* NOTECHANGER handlers */
void
receive_money(int sig) {
    kill(SCREEN_PID, USERSIGNAL1);
}


/* BUTTON handlers */
void
click(int sig) {
    kill(SCREEN_PID, USERSIGNAL2);
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
    printf("*****  STATUS  *****\n");
    kill(SCREEN_PID, SIGINT);
    kill(MECH_PID, SIGINT);
    kill(NOTESCHANGER_PID, SIGINT);
    kill(BUTTON_PID, SIGINT);
    printf("*****  STATUS  *****\n");
}