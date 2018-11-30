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
script_creator( char *script_name, pid_t pid);

int main() {
    int fd = open("hello", O_CREAT, O_RDWR, 0777);
    fprintf(stdout, "228\n");
    if ((MECH_PID = fork()) == 0) {
        signal(SIGUSR1, put_item);
        do {
            pause();
        } while (mech_flag == 1);
        return 0;
    }
    if ((SCREEN_PID = fork()) == 0) {
        //signal(SIGUSR1, deficit_money);
        signal(SIGUSR2, enough_money);
        signal(SIGUSR1, add_money);
        do {
            pause();
        } while (screen_flag == 1);
        return 0;
    }
    //printf("%d", SCREEN_PID);
    if ((BUTTON_PID = fork()) == 0) {
        signal(SIGUSR1, click);
        do {
            pause();
        } while (button_flag == 1);
        return 0;
    }
    if ((NOTESCHANGER_PID = fork()) == 0) {
        //pause();
        signal(SIGUSR1, receive_money);
        do {
            pause();
        } while (notechanger_flag == 1);
        return 0;
    }
    script_writer(SCREEN_PID, MECH_PID, NOTESCHANGER_PID, BUTTON_PID);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}


int
script_writer(pid_t first_pid, pid_t second_pid, pid_t third_pid, pid_t fourth_pid) {
    script_creator("screen_script.sh", first_pid);
    script_creator("mech_script.sh", second_pid);
    script_creator("notechanger_script.sh", third_pid);
    script_creator("button_script.sh", fourth_pid);
    return 0;
}

int
script_creator( char *script_name, pid_t pid) {
    FILE *file = fopen(script_name, "w+");
    char *start_str = "#!/bin/bash\nkill ";
    fprintf(file, "%s", start_str);
    fprintf(file, "%d\n", pid);
    fclose(file);
    return 0;
}