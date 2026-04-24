#include "city_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int parse_condition(const char *input, char *field, char *op, char *value) {
    char temp[256];
    strncpy(temp, input, sizeof(temp));

    char *f = strtok(temp, ":");
    char *o = strtok(NULL, ":");
    char *v = strtok(NULL, ":");

    if (f && o && v) {
        strcpy(field, f);
        strcpy(op, o);
        strcpy(value, v);
        return 1;
    }
    return 0;
}

int match_condition(Report *r, const char *field, const char *op, const char *value) {
    if (strcmp(field, "severity") == 0) {
        int val = atoi(value);
        if (strcmp(op, "==") == 0) return r->severity == val;
        if (strcmp(op, "!=") == 0) return r->severity != val;
        if (strcmp(op, ">") == 0) return r->severity > val;
        if (strcmp(op, ">=") == 0) return r->severity >= val;
        if (strcmp(op, "<") == 0) return r->severity < val;
        if (strcmp(op, "<=") == 0) return r->severity <= val;
    }
    else if (strcmp(field, "timestamp") == 0) {
        long val = atol(value);
        if (strcmp(op, "==") == 0) return r->timestamp == val;
        if (strcmp(op, "!=") == 0) return r->timestamp != val;
        if (strcmp(op, ">") == 0) return r->timestamp > val;
        if (strcmp(op, ">=") == 0) return r->timestamp >= val;
        if (strcmp(op, "<") == 0) return r->timestamp < val;
        if (strcmp(op, "<=") == 0) return r->timestamp <= val;
    }
    else if (strcmp(field, "category") == 0) {
        if (strcmp(op, "==") == 0) return strcmp(r->category, value) == 0;
        if (strcmp(op, "!=") == 0) return strcmp(r->category, value) != 0;
    }
    else if (strcmp(field, "inspector") == 0) {
        if (strcmp(op, "==") == 0) return strcmp(r->inspector, value) == 0;
        if (strcmp(op, "!=") == 0) return strcmp(r->inspector, value) != 0;
    }

    return 0;
}

void filter_reports(const char *district, const char *role, const char *username, int cond_count, char **conditions) {
    char path[256];
    sprintf(path, "%s/reports.dat", district);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("Error: Could not open reports.dat for district '%s'.\n", district);
        return;
    }

    Report r;
    int match_found = 0;

    while (read(fd, &r, sizeof(Report)) > 0) {
        int all_matched = 1;

        for (int i = 0; i < cond_count; i++) {
            char field[50], op[5], value[50];

            if (parse_condition(conditions[i], field, op, value)) {
                if (!match_condition(&r, field, op, value)) {
                    all_matched = 0;
                    break;
                }
            }
        }

        if (all_matched) {
            printf("[ID:%d] %s | Cat:%s | Sev:%d | Desc:%s\n",
                   r.id, r.inspector, r.category, r.severity, r.description);
            match_found++;
        }
    }
    close(fd);

    if (match_found == 0) {
        printf("No reports matched the filter criteria.\n");
    }

    log_action(district, role, username, "filter");
}