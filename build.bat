#/bin/bash

mkdir .build
cd .build
conan install ../conan/ -s compiler.version=15
cmake -G "Visual Studio 15 2017 Win64" ..
cd ..
cmake --build .build --config Release

