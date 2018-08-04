#/bin/bash

mkdir .build
cd .build
conan install ../conan/ -s compiler.version=15 -s arch=x86_64 -s build_type=Release --build=missing
cmake -G "Visual Studio 15 2017 Win64" ..
cd ..
cmake --build .build --config Release

