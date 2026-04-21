#ifndef OPERATIONS_H
#define OPERATIONS_H

void add_report(const char *district, const char *username);
void list_reports(const char *district);
void remove_report(const char *district, const char *role, int target_id);
void update_threshold(const char *district, const char *role, int new_threshold);

#endif