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

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "action.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "ibm4704.h"
#include "ibm4704_interface.h"
#include "matrix.h"
#include "action_layer.h"
#include "unimap_trans.h"
#include "timer.h"


static void matrix_make(uint8_t code);
static void matrix_break(uint8_t code);


/*
 * Matrix Array usage:
 * IBM 4704 scan codes are assigned into 128(16x8)-cell matrix.
 *
 *    8bit wide
 *   +---------+
 *  0|         |
 *  :|   XX    | 00-7F
 *  f|         |
 *   +---------+
 *
 * Exceptions:
 */
static uint8_t matrix[MATRIX_ROWS];

// scan code bits  7654 3210
// R:row/C:column  -RRR RCCC
#define ROW(code)      ((code>>3)&0x0f)
#define COL(code)      (code&0x07)


ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
    timer_count++;
}

uint8_t ibm4704_send_proxy(uint8_t code) {
  return ibm4704_send(code);
}

static void enable_break(void)
{
    print("Enable break: ");
    while (ibm4704_send(0xFC)) { _delay_ms(10); }
    // valid scancode: 00-79h
    for (uint8_t code = 0; code < 0x7F; code++) {
        while (ibm4704_send(0x80|code)) _delay_ms(10);
        _delay_ms(5);   // wait for response
        // No response(FF) when ok, FD when out of bound
        xprintf("s%02X:r%02X ", code, ibm4704_recv());
    }
    while (ibm4704_send(0xFF)) { _delay_ms(10); } // End
    print("End\n");
}


void matrix_setup(void)
{
    ibm4704_init();
}

void matrix_init(void)
{
    matrix_init_ps2();

    debug_enable = true;

    print("IBM 4704 converter\n");
    matrix_clear();
    _delay_ms(1000);
    while (ibm4704_send(0xA3)) {

    };
    _delay_ms(2000);    // wait for keyboard starting up
    uint8_t keyboard_id = ibm4704_recv();
    xprintf("Keyboard ID: %02X\n", keyboard_id);
    //enable_break();
}

/*
* IBM 4704 Scan Code
*/
uint8_t matrix_scan(void)
{
  matrix_scan_ps2();

  //Process commands received from 4704 host
  //TBD

  return 1;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix_get_row_ps2(row);
    return matrix[row];
}

/*
inline
static void matrix_make(uint8_t code)
{
    matrix[ROW(code)] |= 1<<COL(code);
}

inline
static void matrix_break(uint8_t code)
{
    matrix[ROW(code)] &= ~(1<<COL(code));
}
*/

void matrix_clear(void)
{
    matrix_clear_ps2();

    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}



bool hook_process_action(keyrecord_t *record) {

  

  if (!IS_NOEVENT(record->event)) {
    print("PROCESS EVENT ");
    action_t action = layer_switch_get_action(record->event);
    xprintf("KIND: %02X ", action.kind.id);
    if (record->event.pressed) {
      xprintf("PRESSED ", action.kind.id);
    } else {
      xprintf("NOT PRESSED ", action.kind.id);
    }
    xprintf("CODE: %02X ", action.key.code);

    bool found = false;
    uint8_t scancode = 0x00;
    for(int row = 0; row < MATRIX_ROWS_DEST; row++)
    {
      for(int col = 0; col < MATRIX_COLS_DEST; col++)
      {
        uint8_t unimap_code = pgm_read_byte(&unimap_trans_dest[row][col]) ;
        if (unimap_code == action.key.code) {

          //Check if key is registered for make/break (TBD)
          if (record->event.pressed ) {
            scancode = row * 8 + col;
            xprintf("TRANSLATED TO: %02X ", scancode);
            found = true;
          }
          break;
        }
      }

      if (found) {
        break;
      }
    }

    print("\n");

    //Check if Alt.
    //TBD: No idea here how to properly manage through "unimap" and that stuff
    if (!found && (action.key.code == KC_LALT || action.key.code == KC_RALT)) {
      found = true;
      if (record->event.pressed ) {
        scancode = 0xBF;
        print("ALT PRESSED\n");
      } else {
        scancode = 0x3F;
        print("ALT RELEASED\n");
      }
    }


    //Check if RCTRL.
    //TBD: No idea here how to properly manage through "unimap" and that stuff
    if (!found && (action.key.code == KC_RCTRL || action.key.code == KC_RCTRL)) {
      if (record->event.pressed ) {
        scancode = 0x2F;
        print("ENTER PRESSED\n");
        found = true;
      } else {
        print("ENTER RELEASED\n");
      }
    }



    //Check if LCTRL.
    if (!found && (action.key.code == KC_LCTRL || action.key.code == KC_LCTRL)) {
      if (record->event.pressed ) {
        scancode = 0x31;
        print("RESET PRESSED\n");
        found = true;
      } else {
        print("RESET RELEASED\n");
      }
    }




    if (found) {
      //TBD: manage all the other make/break
      ibm4704_send_proxy(scancode);
    }

  }

  return true;
}
