cmake_minimum_required(VERSION 4.0)
project(City_Manager C)

set(CMAKE_C_STANDARD 11)

add_executable(City_Manager
        city_manager_phase1/main.c
        city_manager_phase1/file_io/file_io.c
        city_manager_phase1/operations/operations.c
        city_manager_phase1/permissions/permissions.c
        city_manager_phase1/filter/filter_logic.c)