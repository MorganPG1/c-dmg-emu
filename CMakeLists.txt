cmake_minimum_required(VERSION 3.28)

project(c-dmg-emu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

include_directories(core)

file(GLOB SOURCES "core/*.c")

add_executable(c-dmg-emu main.c ${SOURCES})

target_include_directories(c-dmg-emu PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(c-dmg-emu PRIVATE ${SDL2_LIBRARIES})