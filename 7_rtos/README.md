# Simple self-contained RTOS example

This is a simple example of using the FreeRTOS on the [FRDM-K66F eval
board](https://www.nxp.com/design/development-boards/freedom-development-boards/mcu-boards/freedom-development-platform-for-kinetis-k66-k65-and-k26-mcus:FRDM-K66F).
It demonstrates some of the core functionality of the FreeRTOS operating system.
For more information, check out the sections in the FreeRTOS handbook cited
below [MtFRTK].

The code in `main.c` creates the following tasks before starting the FreeRTOS
scheduler by calling `vTaskStartScheduler()`:

- `vTask1()` prints a message to UART via the debug console, then deletes
  itself.  Normally FreeRTOS tasks run a forever loop, but if they do need to
  exit, they must call `vTaskDelete(NULL)`; "they must not contain a ‘return’
  statement and must not be allowed to execute past the end of the function."
  [MtFRTK sect 3.2]

- `vTask2()`, in its `while (true)` loop, toggles the on/off state of the blue
  LED, then sleeps (enters the Blocked state) for a second using the FreeRTOS
  `vTaskDelay()` function. It never exits the loop. [MtFRTK sect 3.7]

- `prvTimerCallback1()` uses an auto-reload software timer [MtFRTK sect 5.5] to
  blink the red LED.  This is not a task, but rather a callback function to the
  timer, although in this case it behaves similarly as `vTask2()`.

Code should normally check if the tasks and timers were created successfully
using the values returned from `xTaskCreate()` and `xTimerCreate()`, and if the
timers were successfully started using the return value from `xTimerStart`. This
example omits this to make the code easier to read.

## Getting started

Install software:

- [MinGW]( https://sourceforge.net/projects/mingw/) (install the `mingw32-base,
  msys-base` packages)
- [Arm GNU Toolchain](
  https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
  (`arm-gnu-toolchain-12.3.rel1-mingw-w64-i686-arm-none-eabi.exe`)
- [CMake]( https://cmake.org/download/) (`cmake-3.27.4-windows-x86_64.msi`)
- [MCUXpresso Config Tools](
  https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-config-tools-pins-clocks-and-peripherals:MCUXpresso-Config-Tools)

Set environment variables:

- `ARMGCC_DIR` : `C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.3
  rel1`
- `PATH` : `C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.3
  rel1\bin;C:\Program Files\CMake\bin;C:\MinGW\bin`

Start J-Link GDB Server V7.88m. From the command prompt, load code to dev board:

    cd armgcc
    arm-none-eabi-gdb.exe -q -x .\load.gdb

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

- `build_debug.bat`: Running this batch script calls `clean.bat` to delete old
  files, calls CMake to generate make files, then calls make to compile the
  binary file that's ready to be loaded to the microcontroller.

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

- [`add_executable(name, source1,
  ...)`](https://cmake.org/cmake/help/latest/command/add_executable.html):
  Creates an executable target `name` from the source files `source1, ...` The
  `name` must be unique in the project.

- [`add_library(name, source1,
  ...)`](https://cmake.org/cmake/help/latest/command/add_library.html): Creates
  a library target `name` from the source files. The `name` must be unique.

- [`target_include_directories(target, dir1,
  ...)`](https://cmake.org/cmake/help/latest/command/target_include_directories.html):
  When compiling the `target` (as created by either `add_executable` or
  `add_library`), the include directories `dir1, ...` will be used to source the
  `.h` files for the `#include` directives.

- [`target_sources(target, source1,
  ...)`](https://cmake.org/cmake/help/latest/command/target_sources.html): Add
  source files `source1, ...` to the `target` created in a previous invocation
  of `add_executable` or `add_library`.

- [`include(X)`](https://cmake.org/cmake/help/latest/command/include.html):
  Inserts an `X.cmake` file. CMake looks for this file either under its built-in
  modules (in the CMake module directory), or in paths specified in the
  `CMAKE_MODULE_PATH` variable.

- [`add_subdirectory(source_dir,
  binary_dir)`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html):
  Include the module located in `source_dir`. The auto-generated build files
  will be placed in `binary_dir`.

- [`include_directories(dir1,
  ...)`](https://cmake.org/cmake/help/latest/command/include_directories.html):
  Add `dir1, ...` to the list of directories the compiler uses to source the
  `.h` files for the `#include` directives.

- [`target_link_libraries(target, lib1,
  ...)`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html):
  "Specify libraries or flags to use when linking a given target and/or its
  dependents."

## References

[MtFRTK] Richard Barry: *Mastering the FreeRTOS Real Time Kernel - a Hands On
Tutorial Guide.* Available from
<https://freertos.org/Documentation/RTOS_book.html> and accessed on 9/22/2023.
