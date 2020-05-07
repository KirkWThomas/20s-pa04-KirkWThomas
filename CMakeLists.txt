cmake_minimum_required(VERSION 3.12)
project(20S-3353-PA04)
set (EXE_NAME PA04)

##########################################
# Students, edit the items below.  In the quotes,
# put the names of the input files as they should appear
# in the argv param to main.
# Example:
#   set (input01 "small.txt")
#   set (input02 "medium.txt")
#   set (input03 "large.txt")
# etc....

set (input01 "lenna-tga.hpp")
set (input02 "random-512.hpp")

#
##########################################

# Copy data files to build dir
foreach(file IN LISTS input01 input02 input03 input04)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 17)
add_executable(${EXE_NAME} main.cpp)
