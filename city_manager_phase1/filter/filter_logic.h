#ifndef FILTER_LOGIC_H
#define FILTER_LOGIC_H

#include "../report_structs.h"

int parse_condition(const char *input, char *field, char *op, char *value);
int is_match(Report *r, const char *field, const char *op, const char *value);
int filter_all(const char *district_id, int num_conditions, char **condition_strings);

#endif