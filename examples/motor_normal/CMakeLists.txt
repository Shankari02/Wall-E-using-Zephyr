cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr)
project(sra-board_components)

target_sources(app PRIVATE src/mcpwm_interface.c)