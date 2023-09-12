file debug/hello_world.elf
target remote localhost:2331
monitor reset
monitor flash device=MK66FN2M0xxx18
load debug/hello_world.elf
monitor go
detach
