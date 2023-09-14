file debug/LMK05028.elf
target remote localhost:2331
monitor reset
monitor flash device=MK66FN2M0xxx18
load debug/LMK05028.elf
monitor go
detach
