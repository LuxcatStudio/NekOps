cmake_minimum_required(VERSION 3.30)
project(NekOps VERSION 0.7.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_executable(${PROJECT_NAME} src/Program.cpp)

find_package(Boost REQUIRED COMPONENTS system process)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system Boost::process)

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
endif()