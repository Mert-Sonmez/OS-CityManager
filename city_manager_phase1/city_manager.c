#include "city_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char role[20] = "unknown";
    char user[50] = "unknown";
    char action[30] = "none";
    char district[50] = "unknown";
    int target_id = 0;
    int threshold_val = 0;

    char **filter_conditions = NULL;
    int filter_cond_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--role") == 0 && i + 1 < argc) {
            strcpy(role, argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--user") == 0 && i + 1 < argc) {
            strcpy(user, argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--add") == 0 && i + 1 < argc) {
            strcpy(action, "add");
            strcpy(district, argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--list") == 0 && i + 1 < argc) {
            strcpy(action, "list");
            strcpy(district, argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "--view") == 0 && i + 2 < argc) {
            strcpy(action, "view");
            strcpy(district, argv[i + 1]);
            target_id = atoi(argv[i + 2]);
            i += 2;
        } else if (strcmp(argv[i], "--remove_report") == 0 && i + 2 < argc) {
            strcpy(action, "remove_report");
            strcpy(district, argv[i + 1]);
            target_id = atoi(argv[i + 2]);
            i += 2;
        } else if (strcmp(argv[i], "--update_threshold") == 0 && i + 2 < argc) {
            strcpy(action, "update_threshold");
            strcpy(district, argv[i + 1]);
            threshold_val = atoi(argv[i + 2]);
            i += 2;
        } else if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
            strcpy(action, "filter");
            strcpy(district, argv[i + 1]);
            filter_conditions = &argv[i + 2];
            filter_cond_count = argc - (i + 2);
            break;
        }
    }

    check_create_symlink(district);

    if (strcmp(action, "add") == 0) {
        create_district_folder(district);
        setup_files(district);
        add_report(district, role, user);
    }
    else if (strcmp(action, "list") == 0) {
        list_reports(district, role, user);
    }
    else if (strcmp(action, "view") == 0) {
        view_report(district, role, user, target_id);
    }
    else if (strcmp(action, "remove_report") == 0) {
        remove_report(district, role, user, target_id);
    }
    else if (strcmp(action, "update_threshold") == 0) {
        update_threshold(district, role, user, threshold_val);
    }
    else if (strcmp(action, "filter") == 0) {
        filter_reports(district, role, user, filter_cond_count, filter_conditions);
    }
    else {
        printf("Error: Invalid command!\n");
    }

    return 0;
}