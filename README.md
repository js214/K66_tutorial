This contains simple projects using the [NXP K66 Freedom Dev Board](https://www.nxp.com/design/development-boards/freedom-development-boards/mcu-boards/freedom-development-platform-for-kinetis-k66-k65-and-k26-mcus:FRDM-K66F).

### Setup

Install software:

- MinGW (install the `mingw32-base, msys-base` packages)
- Arm GNU Toolchain (`arm-gnu-toolchain-12.3.rel1-mingw-w64-i686-arm-none-eabi.exe`)
- CMake (`cmake-3.27.4-windows-x86_64.msi`)
- MCUXpresso IDE
- Download the SDK for FRDM-K66F

Open file `SDK_2_11_0_FRDM-K66F\devices\MK66F18\utilities\fsl_sbrk.c` and add a line at the start:

    typedef char *caddr_t

Set environment variables:

- `ARMGCC_DIR` C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.3 rel1
- `PATH` C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.3 rel1\bin;C:\Program Files\CMake\bin;C:\MinGW\bin

### Load code to dev board

Open GCC Command Prompt, and go to `led_blinky/armgcc`.

Run `build_debug.bat` to compile the code.

Start J-Link GDB Server V7.88m and select target device	MK66FN2M0xxx18.

Start GDB:

    arm-none-eabi-gdb.exe debug\led_blinky.elf

Connect to J-Link and load code:

    target remote localhost:2331
    monitor reset
    monitor flash device=MK66FN2M0xxx18
    load debug/led_blinky.elf

Run code, stop it, detach GDB, quit:

    monitor go
    monitor halt
    detach
    quit
