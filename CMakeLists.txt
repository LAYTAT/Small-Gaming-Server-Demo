cmake_minimum_required(VERSION 3.0.0)
project(My_Server)
#
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCE_DIR .)

add_subdirectory(login_server)

add_subdirectory(db_server)
