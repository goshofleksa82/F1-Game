cmake_minimum_required(VERSION 3.16)
project(F1Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(F1Game
    src/main.cpp
    src/Game.cpp
    src/Track.cpp
    src/Car.cpp
    src/Race.cpp
)

target_include_directories(F1Game PRIVATE src)
target_link_libraries(F1Game PRIVATE SDL3::SDL3)