#include "file_io.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void create_district_folder(const char *district_name) {
    mkdir(district_name, 0700);

    if (chmod(district_name, 0750) == 0) {
        printf("Folder '%s' is ready and permissions set to 0750.\n", district_name);
    } else {
        printf("Failed to set permissions for folder '%s'.\n", district_name);
    }

    char link_name[128];
    char target_path[128];

    snprintf(link_name, sizeof(link_name), "active_reports-%s", district_name);
    snprintf(target_path, sizeof(target_path), "%s/reports.dat", district_name);

    unlink(link_name);
    if (symlink(target_path, link_name) == 0) {
        printf("Symbolic link created: %s -> %s\n", link_name, target_path);
    }
}

void setup_files(const char *district_name) {
    char filepath[256];

    snprintf(filepath, sizeof(filepath), "%s/reports.dat", district_name);
    int fd1 = open(filepath, O_CREAT | O_RDWR | O_APPEND, 0600);
    if (fd1 != -1) {
        close(fd1);
        chmod(filepath, 0664);
        printf("File created: %s (Permissions: 0664)\n", filepath);
    }

    snprintf(filepath, sizeof(filepath), "%s/district.cfg", district_name);
    int fd2 = open(filepath, O_CREAT | O_RDWR, 0600);
    if (fd2 != -1) {
        close(fd2);
        chmod(filepath, 0640);
        printf("File created: %s (Permissions: 0640)\n", filepath);
    }

    snprintf(filepath, sizeof(filepath), "%s/logged_district", district_name);
    int fd3 = open(filepath, O_CREAT | O_RDWR | O_APPEND, 0600);
    if (fd3 != -1) {
        close(fd3);
        chmod(filepath, 0644);
        printf("File created: %s (Permissions: 0644)\n", filepath);
    }
}