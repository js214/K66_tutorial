file debug/simple.elf
target remote localhost:2331
monitor reset
monitor flash device=MK66FN2M0xxx18
load debug/simple.elf
monitor go
detach
