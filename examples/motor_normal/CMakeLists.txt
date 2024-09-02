cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr)
project(motor-driver-normal-example-app)

target_sources(app PRIVATE main/main.c)