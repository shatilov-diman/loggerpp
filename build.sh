#/bin/bash

mkdir .build
cd .build
conan install ../conan/ -s compiler.version=7 -s arch=x86_64 -s build_type=Release --build=missing
cmake ..
cd ..
cmake --build .build

