cmake_minimum_required(VERSION 3.10)

# set the project name
project(Launcher VERSION 1.0)

set(TC_SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../src")

include_directories(${TC_SRCDIR}/util)

# add the executable
add_executable(Launcher ${TC_SRCDIR}/launchers/linux/Launcher.c)