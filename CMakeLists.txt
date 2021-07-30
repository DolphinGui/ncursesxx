cmake_minimum_required(VERSION 3.14)
cmake_policy(VERSION 3.14)
project(ncurses++ VERSION 0.4.0)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/ncursesxx.hpp)

if(COMMAND NCXX_CHARTYPE)
target_link_libraries(${PROJECT_NAME} ncursesw)
else()
target_link_libraries(${PROJECT_NAME} ncurses)
endif()