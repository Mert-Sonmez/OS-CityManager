#ifndef OPERATIONS_H
#define OPERATIONS_H

void add_report(const char *district, const char *role, const char *username);
void list_reports(const char *district, const char *role, const char *username);
void remove_report(const char *district, const char *role, const char *username, int t_id);
void update_threshold(const char *district, const char *role, const char *username, int val);

#endif