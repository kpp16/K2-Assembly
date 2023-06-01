cmake_minimum_required(VERSION 3.0.0)
project(K2Assembly VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include(CTest)
enable_testing()

include_directories(include)
link_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp" main.cpp)
add_executable(K2Assembly ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
