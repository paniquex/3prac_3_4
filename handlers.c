#include "handlers.h"

int amount = 35;
const ITEM_COST = 20;
enum {NOTE1 = 25, NOTE2 = 50};

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
        kill(MECH_PID, SIGUSR1);
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
    kill(SCREEN_PID, SIGUSR1);
}


/* BUTTON handlers */
void
click(int sig) {
    kill(SCREEN_PID, SIGUSR2);
}


/* SIGTERM */
