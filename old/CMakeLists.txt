#cmake_minimum_required(VERSION 3.4)
#project(AutomataEx)

add_library(AutomataExTest SHARED
    source/main.cc
    source/utilities.cc
)

target_include_directories(AutomataExTest PUBLIC include)