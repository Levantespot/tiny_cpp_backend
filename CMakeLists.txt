cmake_minimum_required(VERSION 3.10)
project(TCB_BASE VERSION 1.0) # set the project name

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions("-Wall -Wextra -g -O2")

# add sub_dir for other CMakeLists
add_subdirectory(base)
# add_subdirectory(net) # TODO