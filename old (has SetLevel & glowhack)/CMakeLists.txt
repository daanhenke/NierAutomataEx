cmake_minimum_required(VERSION 3.4)
project(AutomataEx)

add_library(AutomataEx SHARED
    source/main.cc
    source/utilities.cc
)

target_include_directories(AutomataEx PUBLIC include)