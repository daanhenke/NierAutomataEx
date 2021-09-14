cmake_minimum_required(VERSION 3.19)
project(AutomataEx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#add_subdirectory("old")

add_subdirectory(minhook)
add_subdirectory(DirectXMath)

if (TRUE)
add_library(AutomataEx SHARED
    source/mod.cc

    source/misc/entrypoint.cc
    source/misc/console.cc
    source/misc/directx11.cc
    source/misc/gui.cc
    source/misc/maths.cc
    source/misc/pattern.cc

    source/features/debuggui.cc
    source/features/entityhacks.cc

    source/imgui/imgui.cpp
    source/imgui/imgui_draw.cpp
    source/imgui/imgui_demo.cpp
    source/imgui/imgui_tables.cpp
    source/imgui/imgui_widgets.cpp
    source/imgui/imgui_impl_dx11.cpp
    source/imgui/imgui_impl_win32.cpp
)

target_include_directories(AutomataEx PUBLIC
    include
    include/imgui
)

target_link_libraries(AutomataEx DirectXMath minhook)

endif()