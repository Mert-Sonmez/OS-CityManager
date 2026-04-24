#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_NAME_LEN 50
#define CAT_LEN      30
#define DESC_LEN     256


typedef struct {
    int id;
    char inspector[MAX_NAME_LEN];
    float latitude;
    float longitude;
    char category[CAT_LEN];
    int severity;
    time_t timestamp;
    char description[DESC_LEN];
} Report;

void create_district_folder(const char *district_name);
void setup_files(const char *district_name);
void check_create_symlink(const char *district_name);

void mode_string(mode_t mode, char *str);
void log_action(const char *dist, const char *role, const char *usr, const char *act);
void add_report(const char *district, const char *role, const char *username);
void list_reports(const char *district, const char *role, const char *username);
void view_report(const char *district, const char *role, const char *username, int t_id);
void remove_report(const char *district, const char *role, const char *username, int t_id);
void update_threshold(const char *district, const char *role, const char *username, int val);

int parse_condition(const char *input, char *field, char *op, char *value);
int match_condition(Report *r, const char *field, const char *op, const char *value);
void filter_reports(const char *district, const char *role, const char *username, int cond_count, char **conditions);

#endif