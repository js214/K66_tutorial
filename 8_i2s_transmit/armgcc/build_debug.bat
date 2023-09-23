call clean.bat

cmake -DCMAKE_TOOLCHAIN_FILE="../../../SDK_2_11_0_FRDM-K66F\tools/cmake_toolchain_files/armgcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=debug  .

mingw32-make -j 2> build_log.txt 
