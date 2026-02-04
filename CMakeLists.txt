cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

project(NekOps VERSION 0.7.1 LANGUAGES CXX)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

file(GLOB SOURCE_FILE src/*.cpp)
file(GLOB MODULE_FILE src/modules/*.cppm)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET my_modules TYPE CXX_MODULES
    FILES ${MODULE_FILE}
)

find_package(Boost REQUIRED COMPONENTS system asio)
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system Boost::asio OpenSSL::Crypto)

if(WIN32)
    add_definitions(-D_WIN32 -DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
endif()