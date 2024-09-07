cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(motor-driver-normal-example-app)

set(COMPONENT_ADD_INCLUDEDIRS "include" "src")
set(COMPONENT_SRCS "mcpwm_interface.c")

target_sources(app PRIVATE main/mcpwm.c)