cmake_minimum_required(VERSION 3.23)
project(server_tcp C)

set(CMAKE_C_STANDARD 99)

add_executable(server_tcp main.c client.c server.c)
