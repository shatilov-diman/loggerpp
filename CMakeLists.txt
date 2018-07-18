project(tools)
cmake_minimum_required(VERSION 2.8.12)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(tests
	./tests/main.cpp
	./tests/ordered_queue.cpp
)
target_include_directories(tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(tests ${CONAN_LIBS})

