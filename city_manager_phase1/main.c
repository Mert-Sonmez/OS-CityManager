#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "file_io/file_io.h"
#include "operations/operations.h"

void check_create_symlink(const char *district) {
    if (strcmp(district, "unknown") == 0) return;

    char link_name[256];
    char target_file[256];
    sprintf(link_name, "active_reports-%s", district);
    sprintf(target_file, "%s/reports.dat", district);

    struct stat st;
    if (lstat(link_name, &st) == 0) {
        struct stat target_st;
        if (stat(link_name, &target_st) == -1) {
            printf("Warning: Link exists but reports.dat is missing!\n", link_name);
        }
    } else {
        symlink(target_file, link_name);
    }
}

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
    check_create_symlink(district);

    if (strcmp(action, "add") == 0) {
        create_district_folder(district);
        setup_files(district);

        printf("\n Adding new report \n");
        add_report(district, role, user);
    }
    else if (strcmp(action, "list") == 0) {
        list_reports(district, role, user);
    }
    else if (strcmp(action, "remove_report") == 0) {
        remove_report(district, role, user, target_id);
    }
    else if (strcmp(action, "update_threshold") == 0) {
        update_threshold(district, role, user, threshold_val);
    }

    return 0;
}