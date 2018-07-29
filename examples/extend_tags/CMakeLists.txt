project(loggerpp_sample)
cmake_minimum_required(VERSION 2.8.12)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(loggerpp_sample
	./sample.cpp
)
target_link_libraries(loggerpp_sample ${CONAN_LIBS})

