#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include "handlers.h"

int screen_flag = 1, mech_flag = 1, notechanger_flag = 1, button_flag = 1;

int
script_writer(pid_t first_pid, pid_t second_pid, pid_t third_pid, pid_t fourth_pid);

int
script_creator(const char *script_name, pid_t pid);

int main() {
    fprintf(stdout, "%d\n", getpid());
    if ((MECH_PID = fork()) == 0) {
        signal(USERSIGNAL1, put_item);
        do {
            pause();
        } while (mech_flag == 1);
        return 0;
    }
    if ((SCREEN_PID = fork()) == 0) {
        //signal(USERSIGNAL1, deficit_money);
        signal(USERSIGNAL2, enough_money);
        signal(USERSIGNAL1, add_money);
        do {
            pause();
        } while (screen_flag == 1);
        return 0;
    }
    //printf("%d", SCREEN_PID);
    if ((BUTTON_PID = fork()) == 0) {
        signal(USERSIGNAL1, click);
        do {
            pause();
        } while (button_flag == 1);
        return 0;
    }
    if ((NOTESCHANGER_PID = fork()) == 0) {
        //pause();
        signal(USERSIGNAL1, receive_money);
        do {
            pause();
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
    script_creator(SCREEN_SCRIPT_NAME, first_pid);
    script_creator(MECH_SCRIPT_NAME, second_pid);
    script_creator(NOTESCHANGER_SCRIPT_NAME, third_pid);
    script_creator(BUTTON_SCRIPT_NAME, fourth_pid);
    return 0;
}

int
script_creator(const char *script_name, pid_t pid) {
    FILE *file = fopen(script_name, "w+");
    char *start_str = "#!/bin/bash\nkill ";
    fprintf(file, "%s", start_str);
    fprintf(file, "%d\n", pid);
    fclose(file);
    return 0;
}