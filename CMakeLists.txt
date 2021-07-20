cmake_minimum_required(VERSION 3.8...3.20)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(NAME ogl)

project(${NAME} VERSION 0.0.1 LANGUAGES C)

add_executable(ogl src/ogl.c external/glad/src/gl.c)

target_compile_features(${NAME} PUBLIC c_std_11)
target_compile_options(${NAME} PUBLIC -Wextra -Wall -Wshadow -Wno-missing-braces -Wmissing-field-initializers -fdiagnostics-color=always)

find_package(SDL2 REQUIRED)

target_include_directories(${NAME} PUBLIC src/ external/glad/include)

target_link_libraries(${NAME} PRIVATE SDL2 dl)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()