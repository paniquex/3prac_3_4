#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

pid_t SCREEN_PID, BUTTON_PID, NOTESCHANGER_PID, MECH_PID; //consts, but no??

int main() {
    int fd = open("hello", O_CREAT, O_RDWR, 0777);
    if ((SCREEN_PID = fork()) == 0) {
        //pause();
        return 0;
    }
    write(1, &SCREEN_PID, sizeof(pid_t));
    if ((BUTTON_PID = fork()) == 0) {
        SCREEN_PID = 23;
        write(1, &SCREEN_PID, sizeof(int));
        //pause();
        return 0;
    }
    if ((NOTESCHANGER_PID = fork()) == 0) {
        //pause();
        return 0;
    }
    if ((MECH_PID = fork()) == 0) {
        //pause();
        return 0;
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}
