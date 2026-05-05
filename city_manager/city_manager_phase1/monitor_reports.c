#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

// It deletes the hidden PID file and stops the program
void handle_sigint(int sig) {
    printf("\nMonitor stopping. Received SIGINT.\n");
    unlink(".monitor_pid");
    exit(0);
}

// It prints a message to the screen
void handle_sigusr1(int sig) {
    printf("Notification: A new report was added to a district!\n");
}

// It saves its process ID and waits for signals
int main() {
    struct sigaction sa_int, sa_usr;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_usr.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr.sa_mask);
    sa_usr.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr, NULL);

    int fd = open(".monitor_pid", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd != -1) {
        char buf[32];
        int len = sprintf(buf, "%d\n", getpid());
        write(fd, buf, len);
        close(fd);
    }

    printf("Monitor running with PID: %d\n", getpid());

    while(1) {
        pause();
    }
    return 0;
}