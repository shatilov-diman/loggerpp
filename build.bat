#/bin/bash

BUILD_TYPE=Release

mkdir .build
cd .build
conan install ../conan/conanfile.txt -s compiler.version=15 -s arch=x86_64 -s build_type=%BUILD_TYPE% --build=missing
cmake -G "Visual Studio 15 2017 Win64" -D CMAKE_CXX_FLAGS=/D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING -D CMAKE_CXX_FLAGS=/EHsc -D CMAKE_BUILD_TYPE=%BUILD_TYPE% ..
cd ..
cmake --build .build --config %BUILD_TYPE%

