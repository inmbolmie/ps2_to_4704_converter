/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_PS2_H
#define CONFIG_PS2_H

#include <avr/interrupt.h>


/* matrix size */
#define MATRIX_ROWS 32  // keycode bit: 7-3
#define MATRIX_COLS 8   // keycode bit: 2-0


/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) \
)


//#define NO_SUSPEND_POWER_DOWN


/*
 * PS/2 Busywait
 */
#ifdef PS2_USE_BUSYWAIT
#define PS2_CLOCK_PORT  PORTD
#define PS2_CLOCK_PIN   PIND
#define PS2_CLOCK_DDR   DDRD
#define PS2_CLOCK_BIT   3
#define PS2_DATA_PORT   PORTD
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_BIT    2
#endif

/*
 * PS/2 Pin interrupt
 */
#ifdef PS2_USE_INT
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__)
/* uses INT3 for clock line(ATMega32U4) */
#define PS2_CLOCK_PORT  PORTD
#define PS2_CLOCK_PIN   PIND
#define PS2_CLOCK_DDR   DDRD
#define PS2_CLOCK_BIT   3
#define PS2_DATA_PORT   PORTD
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_BIT    2
#define PS2_INT_INIT()  do {    \
    EICRA |= ((1<<ISC31) |      \
              (0<<ISC30));      \
} while (0)
#define PS2_INT_ON()  do {      \
    EIMSK |= (1<<INT3);         \
} while (0)
#define PS2_INT_OFF() do {      \
    EIMSK &= ~(1<<INT3);        \
} while (0)
#define PS2_INT_VECT    INT3_vect

#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
/* uses PCINT37(PD1) for clock line. this doesn't work with VUSB */
#define PS2_CLOCK_PORT  PORTD
#define PS2_CLOCK_PIN   PIND
#define PS2_CLOCK_DDR   DDRD
#define PS2_CLOCK_BIT   1
#define PS2_DATA_PORT   PORTD
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_BIT    0
#define PS2_INT_INIT()  do {    \
    PCICR  |= (1<<PCIE2);       \
} while (0)
#define PS2_INT_ON()  do {      \
    PCMSK2 |= (1<<PCINT37);     \
} while (0)
#define PS2_INT_OFF() do {      \
    PCMSK2 &= ~(1<<PCINT37);    \
} while (0)
#define PS2_INT_VECT    PCINT2_vect
#endif
#endif


#endif
