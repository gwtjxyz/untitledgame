#!/bin/sh

# compile shaders
glslc shaders/shader.vert -o shaders/compiled/vert.spv
glslc shaders/shader.frag -o shaders/compiled/frag.spv

# configure and build
/usr/bin/cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ -S/home/imp/Development/Personal/untitledgame -B/home/imp/Development/Personal/untitledgame/build -G Ninja
/usr/bin/cmake --build /home/imp/Development/Personal/untitledgame/build --config Debug --target all --
