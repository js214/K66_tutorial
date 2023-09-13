file debug/i2c_interrupt_b2b_transfer_master.elf
target remote localhost:2331
monitor reset
monitor flash device=MK66FN2M0xxx18
load debug/i2c_interrupt_b2b_transfer_master.elf
monitor go
detach
