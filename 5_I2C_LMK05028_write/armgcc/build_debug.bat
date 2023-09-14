if exist release (RD /s /Q debug)
if exist debug (RD /s /Q debug)
if exist CMakeFiles (RD /s /Q CMakeFiles)
if exist Makefile (DEL /s /Q /F Makefile)
if exist cmake_install.cmake (DEL /s /Q /F cmake_install.cmake)
if exist CMakeCache.txt (DEL /s /Q /F CMakeCache.txt)
if exist build_log.txt (DEL /s /Q /F build_log.txt)

cmake -DCMAKE_TOOLCHAIN_FILE="../../../SDK_2_11_0_FRDM-K66F\tools/cmake_toolchain_files/armgcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=debug  .
mingw32-make -j 2> build_log.txt 
