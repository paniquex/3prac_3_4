#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

//int amount = 35;
const int ITEM_COST = 20;

void
deficit_money(int sig) {
    char *notEnoughMoneyStr = "Not enough money\n";
    unsigned int time = 1;
    write(1, notEnoughMoneyStr, strlen(notEnoughMoneyStr));
    sleep(time);
    write(1, notEnoughMoneyStr, strlen(notEnoughMoneyStr));
}

void enough_money(int sig) {
    char *amountStatus = "Current amount: ";
    int amount = 35;
    amount -= ITEM_COST;
    write(1, amountStatus, strlen(amountStatus));
    write(1, "15", 2);
}

int main(void) {
    deficit_money(1);
    enough_money(1);
    return 0;
}
