cmake_minimum_required(VERSION 3.8...3.20)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(NAME ogl)

project(${NAME} VERSION 0.0.1 LANGUAGES C)

find_package(SDL2 CONFIG REQUIRED)

set(HEADERS 
    src/core_types.h
    external/log.c/src/log.h)

set(SOURCES
    src/math.inl
    external/log.c/src/log.c)

add_executable(ogl ${HEADERS} ${SOURCES} "src/ogl.c")


target_include_directories(${NAME} PUBLIC src/ external/glad/include external/log.c/src)
target_link_libraries(${NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)


target_compile_features(${NAME} PUBLIC c_std_11)
target_compile_options(${NAME} PUBLIC -Wextra -Wall -Wshadow -Wno-missing-braces -Wmissing-field-initializers)


# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()