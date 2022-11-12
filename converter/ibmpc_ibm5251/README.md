
PS2 to IBM 5251 keyboard converter
=========================================
NOTE:
====
This code has been modified to be able to be run concurrently with the PS2 Converter defined in the ps2 subdirectory

- 5251 will run on several ports as it is a parallel protocol
- PS/2 will run on ports PD2 (data) and PD3 (clock)

Use pullups if required with PS/2 signals

__Do not apply power from USB and 5251 at the same time__

This is unfinished and experimental code

This converter currently only supports rev1 TMK with atmega32u4

    $ make -f Makefile.rev1

The USB connector can be used to debug the converter with hid_listen, but you have to uncomment this in lufa.c:

    //USB_Init();


[README PS2](./ps2/README.md)
