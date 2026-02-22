cmake_minimum_required(VERSION 3.16)
project(imgui CXX)

add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
)
if(WIN32)
    target_sources(imgui PRIVATE imgui_impl_win32.cpp)
endif()

target_include_directories(imgui PUBLIC .)
target_link_libraries(imgui PUBLIC glfw)

if(WIN32)
    target_link_libraries(imgui PUBLIC opengl32)
else()
    target_link_libraries(imgui PUBLIC GL)
endif()
