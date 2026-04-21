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

void log_action(const char *dist, const char *role, const char *usr, const char *act) {
    char path[256];
    sprintf(path, "%s/logged_district", dist);

    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd == -1) return;

    time_t now = time(NULL);
    char *t = ctime(&now);
    t[strlen(t)-1] = '\0'; // ctime'in sonundaki alt satira gecmeyi siler

    char log[512];
    int len = sprintf(log, "[%s] Role: %s | User: %s | Action: %s\n", t, role, usr, act);
    write(fd, log, len);
    close(fd);
}

void add_report(const char *district, const char *role, const char *username) {
    Report r;
    memset(&r, 0, sizeof(Report));

    r.id = (int)time(NULL);
    strcpy(r.inspector, username);
    r.timestamp = r.id;

    printf("Enter Lat, Lon, Category, Severity (e.g: 45.7 21.2 road 2): ");
    scanf("%f %f %s %d", &r.latitude, &r.longitude, r.category, &r.severity);
    getchar(); // buffer temizle

    printf("Description: ");
    fgets(r.description, sizeof(r.description), stdin);
    r.description[strcspn(r.description, "\n")] = 0;

    char path[256];
    sprintf(path, "%s/reports.dat", district);

    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd != -1) {
        write(fd, &r, sizeof(Report));
        close(fd);
        log_action(district, role, username, "add_report");
        printf("Success: Report added.\n");
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
    int c = 0;
    while (read(fd, &r, sizeof(Report)) > 0) {
        printf("[ID:%d] %s | Cat:%s | Sev:%d | %s\n", r.id, r.inspector, r.category, r.severity, r.description);
        c++;
    }
    close(fd);

    if (c == 0) printf("No reports.\n");
    log_action(district, role, username, "list_reports");
}

void remove_report(const char *district, const char *role, const char *username, int t_id) {
    if (strcmp(role, "manager") != 0) {
        printf("Error: Only manager can remove.\n"); return;
    }

    char path[256];
    sprintf(path, "%s/reports.dat", district);

    int fd = open(path, O_RDWR);
    if (fd == -1) return;

    struct stat st; fstat(fd, &st);
    Report r;
    int found = 0;

    while (read(fd, &r, sizeof(Report)) > 0) {
        if (r.id == t_id) { found = 1; break; }
    }

    if (found) {
        off_t r_cur = lseek(fd, 0, SEEK_CUR);
        off_t w_cur = r_cur - sizeof(Report);

        while (read(fd, &r, sizeof(Report)) > 0) {
            off_t next = lseek(fd, 0, SEEK_CUR);
            lseek(fd, w_cur, SEEK_SET);
            write(fd, &r, sizeof(Report));
            w_cur += sizeof(Report);
            lseek(fd, next, SEEK_SET);
        }
        ftruncate(fd, st.st_size - sizeof(Report));
        printf("Success: Removed ID %d\n", t_id);
        log_action(district, role, username, "remove_report");
    } else {
        printf("Not found.\n");
    }
    close(fd);
}

void update_threshold(const char *district, const char *role, const char *username, int val) {
    if (strcmp(role, "manager") != 0) return;

    char path[256];
    sprintf(path, "%s/district.cfg", district);

    struct stat st; stat(path, &st);
    if ((st.st_mode & 0777) != 0640) {
        printf("Error: Perms must be 0640\n"); return;
    }

    int fd = open(path, O_WRONLY | O_TRUNC);
    if (fd != -1) {
        char buf[50];
        int l = sprintf(buf, "THRESHOLD=%d\n", val);
        write(fd, buf, l);
        close(fd);
        printf("Threshold updated.\n");
        log_action(district, role, username, "update_threshold");
    }
}