#include "city_manager.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void create_district_folder(const char *district_name) {
    mkdir(district_name, 0750);
    chmod(district_name, 0750);
}

void setup_files(const char *district_name) {
    char filepath[256];
    int fd;

    sprintf(filepath, "%s/reports.dat", district_name);
    fd = open(filepath, O_CREAT | O_RDWR, 0664); close(fd);
    chmod(filepath, 0664);

    sprintf(filepath, "%s/district.cfg", district_name);
    fd = open(filepath, O_CREAT | O_RDWR, 0640); close(fd);
    chmod(filepath, 0640);

    sprintf(filepath, "%s/logged_district", district_name);
    fd = open(filepath, O_CREAT | O_RDWR, 0644); close(fd);
    chmod(filepath, 0644);
}

void check_create_symlink(const char *district_name) {
    if (strcmp(district_name, "unknown") == 0) return;

    char link_name[256];
    char target[256];
    sprintf(link_name, "active_reports-%s", district_name);
    sprintf(target, "%s/reports.dat", district_name);

    struct stat st;
    if (lstat(link_name, &st) != 0) {
        symlink(target, link_name);
    }
}