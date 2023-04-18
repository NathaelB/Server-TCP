cmake_minimum_required(VERSION 3.23)
project(server_tcp C)

set(CMAKE_C_STANDARD 99)

add_executable(server main.c server.c server.h)
