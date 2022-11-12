
PS2 to IBM 5251/3278 keyboard converter
=========================================
NOTE:
====
This code has been modified to be able to be run concurrently with the IBM XT/AT/Terminal Converter defined in the ibmpc subdirectory

- 5251/3278 will run on several ports as it is a parallel protocol. More information abouth the converter hardware here: https://github.com/inmbolmie/Save-a-Term
 - PD2 - Clicker
 - PD3-PD9 - scancode 1-7
 - PD10 - buzzer
 - PD11 - breakcode
 - PD12 - 5251 detection
 - PD13 - 3278 detection
 - A0 - delay strobe
 - A1 - data strobe
 - A2 - reset_0
 - A3 - reset_1


- PS/2 will run on ports RXI (data) and TXO (clock)

Use pullups if required with PS/2 signals

__DO NOT apply power from USB and 5251/3278 at the same time__

__DO NOT connect 5251 AND 3278 at the same time__

__DO NOT connect more than one keyboard at the same time__

__DO NOT connect 5251 to the 3278 port or 3278 to the 5251 port (pinouts are different)__

This is unfinished and experimental code, use at your own risk

This converter currently only supports rev1 TMK with atmega32u4

    $ make -f Makefile.rev1

The USB connector can be used to debug the converter with hid_listen, but you have to uncomment this in lufa.c:

    //USB_Init();


[README IBMPC](./ibmpc/README.md)
