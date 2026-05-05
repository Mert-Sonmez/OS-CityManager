cmake_minimum_required(VERSION 4.0)
project(City_Manager C)

set(CMAKE_C_STANDARD 11)

add_executable(City_Manager
        city_manager_phase1/city_manager.c
        city_manager_phase1/file_io.c
        city_manager_phase1/operations.c
        city_manager_phase1/permissions.c
        city_manager_phase1/filter.c
        city_manager_phase1/city_manager.h
        city_manager_phase1/monitor_reports.c)