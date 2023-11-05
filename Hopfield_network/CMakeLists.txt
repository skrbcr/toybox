cmake_minimum_required(VERSION 3.19)
project(Main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(ZLIB REQUIRED)

add_link_options()

add_executable(memorize memorize.cpp)
target_include_directories(memorize PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(memorize ${OpenCV_LIBS} ZLIB::ZLIB)
target_compile_options(memorize PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

add_executable(recall recall.cpp)
target_include_directories(recall PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(recall ${OpenCV_LIBS} ZLIB::ZLIB)
target_compile_options(recall PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

