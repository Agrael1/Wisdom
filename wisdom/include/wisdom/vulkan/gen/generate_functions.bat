@echo off
start /B "" "C:/Users/Agrae/source/repos/Wisdom/out/build/x64-debug-vk/_deps/wisvk-build/WisVk_generator.exe" -l -i C:/Users/Agrae/source/repos/Wisdom/wisdom/include/wisdom/vulkan/gen/vk_functions.in -o C:/Users/Agrae/source/repos/Wisdom/wisdom/include/wisdom/generated/vulkan/vk_functions.hpp %*
