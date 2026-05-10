#include "city_manager.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    char name[50];
    int score;
} InspectorScore;

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;

    char path[256];
    sprintf(path, "%s/reports.dat", argv[1]);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("District %s: No reports found.\n", argv[1]);
        return 0;
    }

    InspectorScore scores[20] = {0};
    int count = 0;
    Report r;

    // Read binary file
    while (read(fd, &r, sizeof(Report)) > 0) {
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(scores[i].name, r.inspector) == 0) {
                scores[i].score += r.severity;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(scores[count].name, r.inspector);
            scores[count].score = r.severity;
            count++;
        }
    }
    close(fd);

    for (int i = 0; i < count; i++) {
        printf("[District: %s] Inspector '%s' Workload Score: %d\n", argv[1], scores[i].name, scores[i].score);
    }

    return 0;
}