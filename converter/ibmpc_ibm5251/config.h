/*
Copyright 2014 Jun Wako <wakojun@gmail.com>

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

#ifndef CONFIG_H
#define CONFIG_H

#define NO_USB_STARTUP_WAIT_LOOP
#define NO_USB_SUSPEND_LOOP

#include <avr/interrupt.h>

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x4707
#define DEVICE_VER      0x0001
#define MANUFACTURER    t.m.k.
#define PRODUCT         IBMPC to IBM 5251 keyboard converter
#define DESCRIPTION     convert IBMPC keyboard to IBM 5251


/* Terminal type detection pin configuration */
#define TERMINAL_TYPE_5251_PORT  PORTB
#define TERMINAL_TYPE_5251_PIN   PINB
#define TERMINAL_TYPE_5251_DDR   DDRB
#define TERMINAL_TYPE_5251_BIT   3

#define TERMINAL_TYPE_3278_PORT  PORTB
#define TERMINAL_TYPE_3278_PIN   PINB
#define TERMINAL_TYPE_3278_DDR   DDRB
#define TERMINAL_TYPE_3278_BIT   1

/* Clicker Pin configuration */
#define CLICKER_PORT  PORTD
#define CLICKER_PIN   PIND
#define CLICKER_DDR   DDRD
#define CLICKER_BIT   1


/* Pin interrupt on rising edge of clock */
#define CLICKER_INT_INIT()  do { EICRA |= _BV(ISC10); } while (0)
#define CLICKER_INT_ON()    do { EIFR |= (1<<INTF1); EIMSK |= _BV(INT1); } while (0)
#define CLICKER_INT_OFF()   do { EIMSK &= ~(1<<INT1); } while (0)
#define CLICKER_INT_VECT    INT1_vect


/* Pin configuration for pro-micro*/
#define IBM5251_STROBE_PORT  PORTF
#define IBM5251_STROBE_PIN   PINF
#define IBM5251_STROBE_DDR   DDRF
#define IBM5251_STROBE_BIT   6

#define IBM5251_STROBE_DELAY_PORT   PORTF
#define IBM5251_STROBE_DELAY_PIN    PINF
#define IBM5251_STROBE_DELAY_DDR    DDRF
#define IBM5251_STROBE_DELAY_BIT    7

#define IBM5251_BREAK0_PORT  PORTB
#define IBM5251_BREAK0_PIN   PINB
#define IBM5251_BREAK0_DDR   DDRB
#define IBM5251_BREAK0_BIT   2

#define IBM5251_DATA1_PORT   PORTD
#define IBM5251_DATA1_PIN    PIND
#define IBM5251_DATA1_DDR    DDRD
#define IBM5251_DATA1_BIT    0

#define IBM5251_DATA2_PORT   PORTD
#define IBM5251_DATA2_PIN    PIND
#define IBM5251_DATA2_DDR    DDRD
#define IBM5251_DATA2_BIT    4

#define IBM5251_DATA3_PORT   PORTC
#define IBM5251_DATA3_PIN    PINC
#define IBM5251_DATA3_DDR    DDRC
#define IBM5251_DATA3_BIT    6

#define IBM5251_DATA4_PORT   PORTD
#define IBM5251_DATA4_PIN    PIND
#define IBM5251_DATA4_DDR    DDRD
#define IBM5251_DATA4_BIT    7

#define IBM5251_DATA5_PORT   PORTE
#define IBM5251_DATA5_PIN    PINE
#define IBM5251_DATA5_DDR    DDRE
#define IBM5251_DATA5_BIT    6

#define IBM5251_DATA6_PORT   PORTB
#define IBM5251_DATA6_PIN    PINB
#define IBM5251_DATA6_DDR    DDRB
#define IBM5251_DATA6_BIT    4

#define IBM5251_DATA7_PORT   PORTB
#define IBM5251_DATA7_PIN    PINB
#define IBM5251_DATA7_DDR    DDRB
#define IBM5251_DATA7_BIT    5

/* matrix size */
#define MATRIX_ROWS_DEST 16  // keycode bit3-6
#define MATRIX_COLS_DEST 8   // keycode bit0-2


//
//IBMPC STUFF
//
//
//


/* matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 16



/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) || \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) \
)


// CS2 80-prefixed codes support
//#define CS2_80CODE_SUPPORT

// G80-2551 terminal keyboard support
//#define G80_2551_SUPPORT

// Mouse Extended Report
//#define MOUSE_EXT_REPORT


/*
 * Pin and interrupt configuration
 */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__) || defined(__AVR_AT90USB1286__)
// clock requires External Interrupt pin(INT*)

#define IBMPC_CLOCK_PORT  PORTD
#define IBMPC_CLOCK_PIN   PIND
#define IBMPC_CLOCK_DDR   DDRD
#define IBMPC_DATA_PORT   PORTD
#define IBMPC_DATA_PIN    PIND
#define IBMPC_DATA_DDR    DDRD

#define IBMPC_CLOCK_BIT  3
#define IBMPC_DATA_BIT   2

#define IBMPC_INT_INIT()  do { \
    EICRA |= ((1<<ISC31) |      \
              (0<<ISC30));      \
} while (0)
#define IBMPC_INT_ON()  do {   \
    EIFR  |= (1<<INTF3);        \
    EIMSK |= (1<<INT3);         \
} while (0)
#define IBMPC_INT_OFF() do {   \
    EIMSK &= ~(1<<INT3);        \
} while (0)
#define IBMPC_INT_VECT   INT3_vect



/* reset line */
#define IBMPC_RST_PORT    PORTF
#define IBMPC_RST_PIN     PINF
#define IBMPC_RST_DDR     DDRF
#define IBMPC_RST_BIT0    5
#define IBMPC_RST_BIT1    4

/* reset for XT Type-1 keyboard: low pulse for 500ms */
#define IBMPC_RST_HIZ() do { \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_DDR  &= ~(1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_DDR  &= ~(1<<IBMPC_RST_BIT1);  \
} while (0)

#define IBMPC_RST_LO() do { \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT0);  \
    IBMPC_RST_PORT &= ~(1<<IBMPC_RST_BIT1);  \
    IBMPC_RST_DDR  |=  (1<<IBMPC_RST_BIT1);  \
} while (0)

// for debug
#define LED_ON()    do { DDRD |= (1<<5); PORTD |=  (1<<5); } while (0)
#define LED_OFF()   do { DDRD |= (1<<5); PORTD &= ~(1<<5); } while (0)

#else
#error "No pin configuration in config.h"
#endif


#endif
