@echo off
start /B "" "C:/Users/Agrael/source/repos/Wisdom/out/build/x64-Debug/_deps/wisvk-build/WisVk_generator.exe" -l -i C:/Users/Agrael/source/repos/Wisdom/wisdom/include/wisdom/vulkan/gen/vk_functions.in -o C:/Users/Agrael/source/repos/Wisdom/wisdom/include/wisdom/generated/vulkan/vk_functions.hpp %*
