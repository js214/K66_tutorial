# Simple self-contained RTOS example

This is a minimal example of using the FreeRTOS. The code in `main.c` starts a
task that toggles the on/off state of an LED present on the FRDM-K66F eval
board.

## Code organization

### Main source code

For this minimal project, all source code (except SDK and RTOS libraries) is
contained in `main.c`.

Certain board-specific configuration is contained in `board.c`, `board.h` (not
sure what exactly!).

In addition, `main.mex` is used with MCUXpresso Config Tools to generate the
files in the `board` directory:

- `clock_config.c`, `clock_config.h`: Clock configuration.

- `pin_mux.c`, `pin_mix.h`: Pin assigmnment.

These files are not edited manually, since Config Tools overwrites them.

### Build files

The directory `armgcc` contains Batch files, CMake files, debugging files. These
are:

- `build_debug.bat`: Running this batch script deletes old files, calls CMake to
  generate make files, then calls make to compile the binary file that's ready
  to be loaded to the microcontroller.

- `clean.bat`: Delete all auto-generated files (i.e., those that are created by
  `build_debug.bat`).

- `CMakeLists.txt`: The main input file to CMake, containing a list of all
  source files and targets, as well as compiler flags and directives.

- `load.gdb`: GDB script that loads the generated binary file onto the
  microcontroller and starts the program execution.

- `flags.cmake`: Flags to be passed to the compiler (copied verbatim from the
  SDK examples).

- `MK66FN2M0xxx18_flash.ld`: Linker configuration (copied verbatim from the SDK
  examples).

### Auto-generated files

Running `build_debug.bat` compiles the project, creating a number of additional
files in `armgcc`. These files can safely be removed using `clean.bat`. The
files are:

- `debug/main.elf`: The final binary file produced by the compilation process.
  This file is to be loaded on the microcontroller.

- `freertos`: Files needed to compile the FreeRTOS kernel.

- `build_log.txt`: Errors from running `mingw32-make` (redirected stderr).

- `Makefile`: The output of CMake and input to `mingw32-make`.

- `CMakeFiles`, `CMakeCache.txt`: Some stuff used by CMake.

- `output.map`: Probably used for debugging?

### SDK library files

The NXP Kinetis SDK defines a number of useful drivers and utilities. The
following directories contain copies of files from the SDK distribution:

- `drivers`: A collection of drivers for features like I2C or UART. These come
  as `fsl_*.c`, `fsl*.h` files, together with compilation instructions in the
  corresponding `driver_*_MK66F18.cmake` file. The latter enables the drivers to
  be included in the compilation of the main project using `include` statements
  in the `CMakeLists.txt` file.

- `utilities`: The debug console defines versions of `printf` and `scanf` that
  print to UART. The `fsl_assert.c` defines assert functionality. Misc utilities
  include `fsl_sbrk.c`.

- `CMSIS`: "CMSIS provides interfaces to processor and peripherals, real-time
  operating systems, and middleware components" according to the [ARM
  website](https://www.keil.arm.com/cmsis). Not sure what it actually does.

- `components`: UART, list. Not sure what this is for exactly. I thought UART
  functionality is already defined in `drivers`?

- `device`: Obviously device-specific stuff, but no clue what it really does!

### RTOS

"FreeRTOS is a market-leading real-time operating system (RTOS) for
microcontrollers and small microprocessors." [[FreeRTOS
website](https://www.freertos.org/index.html)]

All the files that implement the RTOS kernel are contained in `freertos`. The
configuration of the kernel is done in `freertos/portable/FreeRTOSConfig.h`.

## CMake notes

To include `.c` and `.h` files into the project, CMake provides several
similar-looking but subtly different commands:

- `add_executable(name, source1, ...)`: Creates an executable target `name` from
  the source files `source1, ...` The `name` must be unique in the project.

- `add_library(name, source1, ...)`: Creates a library target `name` from the
  source files. The `name` must be unique.

- `target_include_directories(target, dir1, ...)`: When compiling the `target`
  (as created by either `add_executable` or `add_library`), the include
  directories `dir1, ...` will be used to source the `.h` files for the
  `#include` directives.

- `target_sources(target, source1, ...)`: Add source files `source1, ...` to the
  `target` created in a previous invocation of `add_executable` or
  `add_library`.

- `include(X)`: Inserts an `X.cmake` file. CMake looks for this file either
  under its built-in modules (in the CMake module directory), or in paths
  specified in the `CMAKE_MODULE_PATH` variable.

- `add_subdirectory(source_dir, binary_dir)`: Include the module located in
  `source_dir`. The auto-generated build files will be placed in `binary_dir`.

- `include_directories(dir1, ...)`: Add `dir1, ...` to the list of directories
  the compiler uses to source the `.h` files for the `#include` directives.

- [`target_link_libraries(target, lib1,
  ...)`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html):
  "Specify libraries or flags to use when linking a given target and/or its
  dependents."
