cmake . --preset x64-debug-only-lib
cd build/debug
cmake --build .

cd ../../
cmake . --preset x64-release-only-lib
cd build/release
cmake --build .

cd ../

copy ..\cmake\multi-config.cmake
cpack --config multi-config.cmake -G ZIP
cpack --config multi-config.cmake -G NuGet
