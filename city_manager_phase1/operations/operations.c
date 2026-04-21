#include "operations.h"
#include "../report_structs.h"
#include "../permissions/permissions.h"
#include "../filter/filter_logic.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void add_report(const char *district, const char *username) {
    Report new_report;
    memset(&new_report, 0, sizeof(Report));

    new_report.id = (int)time(NULL);
    strncpy(new_report.inspector, username, sizeof(new_report.inspector) - 1);
    new_report.timestamp = time(NULL);

    printf("X: ");
    scanf("%f", &new_report.latitude);

    printf("Y: ");
    scanf("%f", &new_report.longitude);

    printf("Category (road/lighting/flooding/other): ");
    scanf("%29s", new_report.category);

    printf("Severity level (1/2/3): ");
    scanf("%d", &new_report.severity);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Description: ");
    fgets(new_report.description, sizeof(new_report.description), stdin);
    new_report.description[strcspn(new_report.description, "\n")] = 0;

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/reports.dat", district);

    int fd = open(filepath, O_WRONLY | O_APPEND);
    if (fd == -1) {
        printf("Error: Could not open reports.dat file!\n");
        return;
    }

    write(fd, &new_report, sizeof(Report));
    close(fd);

    printf("\nSuccess: Report has been added to %s/reports.dat\n", district);
}

void list_reports(const char *district) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/reports.dat", district);

    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        printf("Error: Could not find district or reports file!\n");
        return;
    }

    char perms[10];
    mode_string(file_stat.st_mode, perms);

    char time_str[100];
    struct tm *tm_info = localtime(&file_stat.st_mtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("\nFile Info: %s\n", filepath);
    printf("Permissions: %s\n", perms);
    printf("Size: %lld bytes\n", (long long)file_stat.st_size);
    printf("Last modified: %s\n", time_str);

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    Report temp_report;
    int count = 0;

    printf("Reports List\n");

    while (read(fd, &temp_report, sizeof(Report)) == sizeof(Report)) {
        printf("[ID: %d] Inspector: %s | Cat: %s | Sev: %d | Desc: %s\n",
               temp_report.id,
               temp_report.inspector,
               temp_report.category,
               temp_report.severity,
               temp_report.description);
        count++;
    }
    close(fd);

    if (count == 0) {
        printf("No reports found in this district.\n");
    }
}
void remove_report(const char *district, const char *role, int target_id) {
    if (strcmp(role, "manager") != 0) {
        printf("Error: Only 'manager' can remove reports. You are '%s'.\n", role);
        return;
    }

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/reports.dat", district);

    int fd = open(filepath, O_RDWR);
    if (fd == -1) {
        printf("Error: Could not open file for modification.\n");
        return;
    }

    struct stat st;
    fstat(fd, &st);
    off_t original_size = st.st_size;

    Report temp;
    int found = 0;

    while (read(fd, &temp, sizeof(Report)) == sizeof(Report)) {
        if (temp.id == target_id) {
            found = 1;
            break;
        }
    }

    if (found) {
        off_t read_cursor = lseek(fd, 0, SEEK_CUR);

        off_t write_cursor = read_cursor - sizeof(Report);

        while (read(fd, &temp, sizeof(Report)) == sizeof(Report)) {
            off_t next_read = lseek(fd, 0, SEEK_CUR);

            lseek(fd, write_cursor, SEEK_SET);
            write(fd, &temp, sizeof(Report));

            write_cursor += sizeof(Report);
            lseek(fd, next_read, SEEK_SET);
        }

        ftruncate(fd, original_size - sizeof(Report));
        printf("Success: Report ID %d has been removed.\n", target_id);
    } else {
        printf("Error: Report ID %d not found in this district.\n", target_id);
    }

    close(fd);
}
void update_threshold(const char *district, const char *role, int new_threshold) {
    if (strcmp(role, "manager") != 0) {
        printf("Error: Only 'manager' can update the threshold.\n");
        return;
    }

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/district.cfg", district);

    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        printf("Error: Could not find %s\n", filepath);
        return;
    }

    mode_t permissions = file_stat.st_mode & 0777;
    if (permissions != 0640) {
        printf("Error: Permission mismatch! district.cfg must be 0640, but someone altered it.\n");
        return;
    }

    int fd = open(filepath, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        printf("Error: Could not open district.cfg for writing.\n");
        return;
    }

    char buffer[50];
    int len = snprintf(buffer, sizeof(buffer), "THRESHOLD=%d\n", new_threshold);
    write(fd, buffer, len);
    close(fd);

    printf("Success: Severity threshold updated to %d for %s\n", new_threshold, district);
}

void filter_reports(const char *district, int argc, char *argv[], int start_index) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/reports.dat", district);

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) return;

    Report r;
    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        int all_match = 1;
        for (int i = start_index; i < argc; i++) {
            char field[50], op[10], val[50];
            if (parse_condition(argv[i], field, op, val)) {
                if (!is_match(&r, field, op, val)) {
                    all_match = 0;
                    break;
                }
            }
        }
        if (all_match) {
            printf("[MATCHED ID: %d] Cat: %s | Sev: %d\n", r.id, r.category, r.severity);
        }
    }
    close(fd);
}