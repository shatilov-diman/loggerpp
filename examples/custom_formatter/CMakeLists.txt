project(loggerpp_sample)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(loggerpp_sample
	./sample.cpp
)
target_link_libraries(loggerpp_sample ${CONAN_LIBS})

