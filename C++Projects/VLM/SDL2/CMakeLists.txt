
# minimum version needed
cmake_minimum_required(VERSION 3.16)

# project name with used language
project(sound LANGUAGES CXX)

# set C++17 as standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# this is for whisper build
# we don't need examples, servers build so OFF: only need whisper library
set(WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WHISPER_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(WHISPER_BUILD_SERVER   OFF CACHE BOOL "" FORCE)

# makes CMake process whisper.cpp/CMakeLists.txt as part project
# it defines the `whisper` target
add_subdirectory(whisper.cpp)

# produce exe for the whisper_pgm
add_executable(whisper_pgm whisper_pgm.cpp)

# name of exe to produce and source files needed for compilation
add_executable(sound SDL2_record.cpp)

# example program to test record.h module
add_executable(record_test record_test.cpp record.cpp)

# voice_text program: records voice and translates to text
add_executable(voice_text voice_text.cpp record.cpp)


# find the SDL2 package: the header files and lib location and other link informations
# REQUIRED: reports error when package is missing
find_package(SDL2 REQUIRED)

# include header files
target_include_directories(sound PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


# find whisper library for the whisper pgm
target_link_libraries(whisper_pgm PRIVATE whisper)

# link the targets with SDL2 library
# PRIVATE: means only our target uses this library

target_link_libraries(sound PRIVATE SDL2::SDL2)
target_link_libraries(record_test PRIVATE SDL2::SDL2)
target_link_libraries(voice_text PRIVATE SDL2::SDL2 whisper)
