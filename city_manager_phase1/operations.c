#include "city_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void log_action(const char *dist, const char *role, const char *usr, const char *act) {
    char path[256];
    sprintf(path, "%s/logged_district", dist);

    if (strcmp(role, "inspector") == 0) {
        printf("Permission Denied: Inspector cannot write to log.\n");
        return;
    }

    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd == -1) return;

    time_t now = time(NULL);
    char *t = ctime(&now);
    t[strlen(t)-1] = '\0';

    char log[512];
    int len = sprintf(log, "[%s] %s (%s): %s\n", t, usr, role, act);
    write(fd, log, len);
    close(fd);
}

void add_report(const char *district, const char *role, const char *username) {
    Report r;
    memset(&r, 0, sizeof(Report));

    r.id = (int)time(NULL);
    strcpy(r.inspector, username);
    r.timestamp = r.id;

    printf("Enter Lat Lon Category Severity: ");
    scanf("%f %f %s %d", &r.latitude, &r.longitude, r.category, &r.severity);
    getchar(); // scanf sonrası buffer temizliği

    printf("Description: ");
    fgets(r.description, sizeof(r.description), stdin);
    r.description[strcspn(r.description, "\n")] = 0;

    char path[256];
    sprintf(path, "%s/reports.dat", district);

    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd != -1) {
        write(fd, &r, sizeof(Report));
        close(fd);
        printf("Report saved.\n");
        log_action(district, role, username, "add");
    }
}

void list_reports(const char *district, const char *role, const char *username) {
    char path[256];
    sprintf(path, "%s/reports.dat", district);

    struct stat st;
    if (stat(path, &st) == -1) return;

    char perms[10];
    mode_string(st.st_mode, perms);
    printf("File: %s | Perms: %s | Size: %lld bytes\n", path, perms, (long long)st.st_size);

    int fd = open(path, O_RDONLY);
    if (fd == -1) return;

    Report r;
    while (read(fd, &r, sizeof(Report)) > 0) {
        printf("[ID:%d] %s - %s (Sev:%d)\n", r.id, r.category, r.inspector, r.severity);
    }
    close(fd);
    log_action(district, role, username, "list");
}

void remove_report(const char *district, const char *role, const char *username, int t_id) {
    if (strcmp(role, "manager") != 0) {
        printf("Error: Manager only action.\n"); return;
    }

    char path[256];
    sprintf(path, "%s/reports.dat", district);
    int fd = open(path, O_RDWR);
    if (fd == -1) return;

    struct stat st;
    fstat(fd, &st);
    Report r;
    int found = 0;
    while (read(fd, &r, sizeof(Report)) > 0) {
        if (r.id == t_id) { found = 1; break; }
    }

    if (found) {
        off_t current_pos = lseek(fd, 0, SEEK_CUR);
        off_t write_pos = current_pos - sizeof(Report);

        while (read(fd, &r, sizeof(Report)) > 0) {
            off_t next_read = lseek(fd, 0, SEEK_CUR);
            lseek(fd, write_pos, SEEK_SET);
            write(fd, &r, sizeof(Report));
            write_pos += sizeof(Report);
            lseek(fd, next_read, SEEK_SET);
        }
        ftruncate(fd, st.st_size - sizeof(Report));
        printf("Report %d removed.\n", t_id);
        log_action(district, role, username, "remove");
    }
    close(fd);
}

void update_threshold(const char *district, const char *role, const char *username, int val) {
    if (strcmp(role, "manager") != 0) return;

    char path[256];
    sprintf(path, "%s/district.cfg", district);

    int fd = open(path, O_WRONLY | O_TRUNC);
    if (fd != -1) {
        char buf[32];
        int l = sprintf(buf, "THRESHOLD=%d\n", val);
        write(fd, buf, l);
        close(fd);
        printf("Threshold updated to %d.\n", val);
        log_action(district, role, username, "update_cfg");
    }
}
void view_report(const char *district, const char *role, const char *username, int t_id) {
    char path[256];
    sprintf(path, "%s/reports.dat", district);
    int fd = open(path, O_RDONLY);
    if (fd == -1) return;

    Report r;
    int found = 0;
    while (read(fd, &r, sizeof(Report)) > 0) {
        if (r.id == t_id) {
            printf("\nReport Details\nID: %d\nInspector: %s\nCategory: %s\nSeverity: %d\nDesc: %s\n",
                   r.id, r.inspector, r.category, r.severity, r.description);
            found = 1; break;
        }
    }
    close(fd);
    if (!found) printf("Report not found.\n");
    log_action(district, role, username, "view");
}