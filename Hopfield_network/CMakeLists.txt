cmake_minimum_required(VERSION 3.19)
project(Hopfield_network)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(interaction interaction.cpp)
target_include_directories(interaction PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(interaction ${OpenCV_LIBS} ZLIB::ZLIB)
target_compile_options(interaction PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

add_executable(movie movie.cpp)
target_include_directories(movie PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(movie ${OpenCV_LIBS} ZLIB::ZLIB)
target_compile_options(movie PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

