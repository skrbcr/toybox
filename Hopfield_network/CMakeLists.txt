cmake_minimum_required(VERSION 3.19)
project(Main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter Development.Embed NumPy)
find_package(OpenCV REQUIRED)
find_package(ZLIB REQUIRED)

add_link_options()

if (Python3_FOUND)
    add_executable(match match.cpp)
    target_include_directories(match PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
    target_link_libraries(match Python3::Python Python3::Module Python3::NumPy -lpthread -lutil -ldl -Xlinker -export-dynamic)
    target_compile_options(match PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)
endif()

add_executable(memorize memorize.cpp)
target_include_directories(memorize PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(memorize ${OpenCV_LIBS} ZLIB::ZLIB)
target_compile_options(memorize PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

add_executable(recall recall.cpp)
target_include_directories(recall PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(recall ${OpenCV_LIBS} ZLIB::ZLIB)
target_compile_options(recall PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

add_executable(zlibtest zlibtest.cpp)
target_link_libraries(zlibtest ZLIB::ZLIB)
target_compile_options(zlibtest PRIVATE $<$<CONFIG:Debug>: -Wall -Wextra -O0 -g3 -ggdb> $<$<CONFIG:Release>: -O3>)

