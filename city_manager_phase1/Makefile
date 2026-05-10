CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Phase 1: city_manager
SRCS = city_manager.c file_io.c operations.c permissions.c filter.c
OBJS = $(SRCS:.c=.o)

# Tüm programları aynı anda derlemek için 'all' kuralı
all: city_manager monitor_reports scorer city_hub

city_manager: $(OBJS)
	$(CC) $(CFLAGS) -o city_manager $(OBJS)

%.o: %.c city_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

# Phase 2: monitor_reports
monitor_reports: monitor_reports.c
	$(CC) $(CFLAGS) monitor_reports.c -o monitor_reports

# Phase 3: scorer
scorer: scorer.c city_manager.h
	$(CC) $(CFLAGS) scorer.c -o scorer

# Phase 3: city_hub
city_hub: city_hub.c
	$(CC) $(CFLAGS) city_hub.c -o city_hub

# Temizlik kuralı
clean:
	rm -f $(OBJS) city_manager monitor_reports scorer city_hub .monitor_pid
	rm -rf active_reports-*