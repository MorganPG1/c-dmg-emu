cmake_minimum_required(VERSION 3.28)

project(c-dmg-emu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(core)

file(GLOB SOURCES "core/*.c")

add_executable(c-dmg-emu main.c ${SOURCES})