set pagination off
file debug/main.elf
target remote localhost:2331
monitor reset
monitor flash device=MK66FN2M0xxx18
load debug/main.elf
monitor go
detach
quit
