#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_io/file_io.h"
#include "operations/operations.h"

int main(int argc, char *argv[]) {
    char role[20] = "unknown";
    char user[50] = "unknown";
    char action[30] = "none";
    char district[50] = "unknown";
    int target_id = 0;
    int threshold_val = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--role") == 0 && i + 1 < argc) {
            strcpy(role, argv[i + 1]);
        }
        else if (strcmp(argv[i], "--user") == 0 && i + 1 < argc) {
            strcpy(user, argv[i + 1]);
        }
        else if (strcmp(argv[i], "--add") == 0 && i + 1 < argc) {
            strcpy(action, "add");
            strcpy(district, argv[i + 1]);
        }
        else if (strcmp(argv[i], "--list") == 0 && i + 1 < argc) {
            strcpy(action, "list");
            strcpy(district, argv[i + 1]);
        }
        else if (strcmp(argv[i], "--remove_report") == 0 && i + 2 < argc) {
            strcpy(action, "remove_report");
            strcpy(district, argv[i + 1]);
            target_id = atoi(argv[i + 2]);
        }
        else if (strcmp(argv[i], "--update_threshold") == 0 && i + 2 < argc) {
            strcpy(action, "update_threshold");
            strcpy(district, argv[i + 1]);
            threshold_val = atoi(argv[i + 2]);
        }
    }

    printf("Role logging into the system: %s\n", role);
    printf("User name: %s\n", user);
    printf("Action to be performed: %s\n", action);
    printf("Target district: %s\n", district);

    if (strcmp(action, "add") == 0) {
        create_district_folder(district);
        setup_files(district);

        printf("\n Adding new report \n");
        add_report(district, user);
    }
    else if (strcmp(action, "list") == 0) {
        list_reports(district);
    }
    else if (strcmp(action, "remove_report") == 0) {
        remove_report(district, role, target_id);
    }
    else if (strcmp(action, "update_threshold") == 0) {
        update_threshold(district, role, threshold_val);
    }

    return 0;
}