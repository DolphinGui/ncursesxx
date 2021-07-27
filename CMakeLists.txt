cmake_minimum_required(VERSION 3.14)
cmake_policy(VERSION 3.14)
project(ncurses++ VERSION 0.3)
include_directories(${CMAKE_SOURCE_DIR}/includes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/)
target_link_libraries(${PROJECT_NAME} ncurses)
add_executable(test1 test/test.cpp)
target_link_libraries(test1 ${PROJECT_NAME})