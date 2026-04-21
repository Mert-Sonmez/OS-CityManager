#include "filter_logic.h"
#include <string.h>
#include <stdlib.h>

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

int is_match(Report *r, const char *field, const char *op, const char *value) {
    if (strcmp(field, "severity") == 0) {
        int val = atoi(value);
        if (strcmp(op, "==") == 0) return r->severity == val;
        if (strcmp(op, "!=") == 0) return r->severity != val;
        if (strcmp(op, ">") == 0) return r->severity > val;
        if (strcmp(op, ">=") == 0) return r->severity >= val;
        if (strcmp(op, "<") == 0) return r->severity < val;
        if (strcmp(op, "<=") == 0) return r->severity <= val;
    } else if (strcmp(field, "category") == 0) {
        if (strcmp(op, "==") == 0) return strcmp(r->category, value) == 0;
        if (strcmp(op, "!=") == 0) return strcmp(r->category, value) != 0;
    }
    return 0;
}