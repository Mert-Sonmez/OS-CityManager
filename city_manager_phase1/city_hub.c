#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void start_monitor() {
    int fd[2];
    pipe(fd);
    pid_t hub_mon = fork();

    if (hub_mon == 0) {
        pid_t monitor_pid = fork();
        if (monitor_pid == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execlp("./monitor_reports", "./monitor_reports", NULL);
            exit(1);
        } else {
            close(fd[1]);
            char buf[256];
            int n;
            while ((n = read(fd[0], buf, sizeof(buf)-1)) > 0) {
                buf[n] = '\0';
                printf("\n[HUB] Monitor says: %s", buf);
            }
            close(fd[0]);
            wait(NULL);
            printf("\n[HUB ALERT] The monitor process has ended!\n");
            exit(0);
        }
    }
    printf("Background monitor (hub_mon) started.\n");
}

void calculate_scores(char *args) {
    char *district = strtok(args, " \n");

    while (district != NULL) {
        int fd[2];
        pipe(fd);
        pid_t pid = fork();

        if (pid == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execlp("./scorer", "./scorer", district, NULL);
            exit(1);
        } else {
            close(fd[1]);
            char buf[512];
            int n;
            wait(NULL);
            while ((n = read(fd[0], buf, sizeof(buf)-1)) > 0) {
                buf[n] = '\0';
                printf("%s", buf);
            }
            close(fd[0]);
        }
        district = strtok(NULL, " \n");
    }
}

int main() {
    char input[256];
    printf("-City Hub Interface-\n");
    printf("Commands: start_monitor, calculate_scores <d1> <d2>, exit\n");

    while (1) {
        printf("hub> ");
        fgets(input, sizeof(input), stdin);

        if (strncmp(input, "exit", 4) == 0) break;

        if (strncmp(input, "start_monitor", 13) == 0) {
            start_monitor();
        } else if (strncmp(input, "calculate_scores", 16) == 0) {
            calculate_scores(input + 17);
        }
    }
    return 0;
}