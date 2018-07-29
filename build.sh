#/bin/bash

mkdir .build
cd .build
conan install ../conan/ -s compiler.version=7
cmake ..
cd ..
cmake --build .build

