@echo off
glslc.exe shaders/shader.vert -o shaders/compiled/vert.spv
glslc.exe shaders/shader.frag -o shaders/compiled/frag.spv
pause