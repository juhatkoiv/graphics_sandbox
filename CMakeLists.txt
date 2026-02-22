cmake_minimum_required(VERSION 3.16)

# vcpkg toolchain — must come before project()
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_SOURCE_DIR}/third_party/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
endif()

project(GraphicsSandbox CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg-managed packages
find_package(glfw3   CONFIG REQUIRED)
find_package(assimp  CONFIG REQUIRED)
find_package(GLEW    REQUIRED)
find_package(unofficial-shaderc CONFIG REQUIRED)

# Source-built third-party
add_subdirectory(third_party/Hashing)
add_subdirectory(third_party/imgui)
add_subdirectory(third_party/tracy)

add_subdirectory(Application)
