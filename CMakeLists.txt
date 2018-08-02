project(loggerpp)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(tests
	./tests/main.cpp
	./tests/logger.cpp
	./tests/worker.cpp
	./tests/task_queue.cpp
	./tests/ordered_queue.cpp
)
target_include_directories(tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(tests ${CONAN_LIBS})

