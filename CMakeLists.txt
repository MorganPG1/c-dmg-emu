cmake_minimum_required(VERSION 3.28)

project(c_dmg_emu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(core)

file(GLOB SOURCES "core/*.c")

add_executable(dmg_emu_c main.c ${SOURCES})