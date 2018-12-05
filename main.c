#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include "handlers.h"

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
        } while (mech_flag == 1);
        return 0;
    }


    if ((SCREEN_PID = fork()) == 0) {
        close(fd[1]);
        read(fd[0], &BUTTON_PID, sizeof(pid_t));
        close(fd[0]);
        //signal(USERSIGNAL1, deficit_money);
        signal(USERSIGNAL2, enough_money);
        signal(USERSIGNAL1, add_money);
        signal(USERSIGNAL4, add_money);
        signal(SIGINT, SIG_IGN); //ignore SIGINT
        signal(USERSIGNAL3, status_screen);
        do {
            pause();
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