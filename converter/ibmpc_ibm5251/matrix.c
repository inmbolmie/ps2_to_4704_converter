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
#include "timer.h"
#include "ibm5251_reversed.h"
#include "ibm5251_interface.h"
#include "matrix.h"
#include "action_layer.h"
#include "unimap_trans.h"
#include "ibmpc_interface.h"
#include <avr/interrupt.h>

static void matrix_make(uint8_t code);
static void matrix_break(uint8_t code);



/*
 * Matrix Array usage:
 * IBM 5251 scan codes are assigned into 128(16x8)-cell matrix.
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
//static uint8_t matrix[MATRIX_ROWS];


// scan code bits  7654 3210
// R:row/C:column  -RRR RCCC
#define ROW(code)      ((code>>3)&0x0f)
#define COL(code)      (code&0x07)


uint8_t ibm5251_send_proxy(uint8_t code) {
  xprintf("SENDING TO HOST TO: %02X\n", code);
  return ibm5251_send(code);
}

long buzzer_cycles = 0;
long typematic_cycles = 0;
bool typematic_enabled = false;
uint8_t typematic_data = false;

void matrix_setup(void)
{
    ibm5251_init();
}

void matrix_init(void)
{
    debug_enable = false;

    //_delay_ms(1000);

    xprintf("IBM 5251 converter\n");

    matrix_init_ibmpc();

    xprintf("ibmpc matrix initialized\n");

    matrix_clear();

    xprintf("clear matrix\n");

    //_delay_ms(2000);    // wait for keyboard starting up

    xprintf("Keyboard initialized\n");

    //_delay_ms(1000);
    buzzer_cycles =100;

}


// excecuted once per 1ms.(excess for just timer count?)
ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{

  timer_count++;

  DDRB |= (1<<6);

  if (buzzer_cycles-- > 0) {

    if ( PINB & (1<<6) ) {
      PORTB &= ~(1<<6);
    } else {
      PORTB |= (1<<6);
    }
  }  else {
    PORTB &= ~(1<<6);
  }


  if (typematic_enabled) {
    if (--typematic_cycles <= 0) {
      typematic_cycles = 30;
      ibm5251_send_proxy(typematic_data);
    }
  }
}




ISR(CLICKER_INT_VECT)
{
  buzzer_cycles =100;
}

/*
* IBM 5251 Scan Code
*/
uint8_t matrix_scan(void)
{
  matrix_scan_ibmpc();

  //Process commands received from 5251 host
  //TBD

  return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix_get_row_ibmpc(row);
    //return matrix[row];
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
    matrix_clear_ibmpc();

    //for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}



//identify 5251 typematic keys
bool is_typematic(uint8_t code) {
  if (
    (code == KC_LCTRL)
    || (code == KC_RCTRL)
    || (code == KC_F1)
    || (code == KC_F2)
    || (code == KC_F3)
    || (code == KC_F4)
    || (code == KC_F5)
    || (code == KC_F6)
    || (code == KC_ENTER)
    || (code == KC_KP_ENTER)
    || (code == KC_KP_PLUS)
    || (code == KC_RSHIFT)
    || (code == KC_RSHIFT)
    || (code == KC_CAPSLOCK)
  ) {
    return false;
  }

  return true;
}

static bool shiftPressed = false;
static bool altPressed = false;

bool hook_process_action(keyrecord_t *record) {

  bool found = false;
  uint8_t scancode = 0x00;

  if (!IS_NOEVENT(record->event)) {
    print("PROCESS EVENT ");

    bool is3278 = (TERMINAL_TYPE_3278_PIN & (1<<TERMINAL_TYPE_3278_BIT));

    action_t action = layer_switch_get_action(record->event);
    xprintf("KIND: %02X ", action.kind.id);
    xprintf("CODE: %02X ", action.key.code);

    //Check if LSHIFT.
    //TBD: No idea here how to properly manage through "unimap" and that stuff
    if (!found && (action.key.code == KC_LSHIFT)) {
      if (record->event.pressed ) {
        if (is3278) {
          scancode = 0x59;
        } else {
          scancode = 0x57;
        }
        print("LSHIFT PRESSED\n");
        shiftPressed = true;
        found = true;
      } else {
        if (is3278) {
          scancode = 0xD9;
        } else {
          scancode = 0xD7;
        }
        print("LSHIFT RELEASED\n");
        shiftPressed = false;
        found = true;
      }
    }


    //Check if RSHIFT.
    if (!found && (action.key.code == KC_RSHIFT)) {
      if (record->event.pressed ) {
        if (is3278) {
          scancode = 0x39;
        } else {
          scancode = 0x56;
        }
        print("RSHIFT PRESSED\n");
        shiftPressed = true;
        found = true;
      } else {
        if (is3278) {
          scancode = 0xB9;
        } else {
          scancode = 0xD6;
        }
        print("RSHIFT RELEASED\n");
        shiftPressed = false;
        found = true;
      }
    }


    //Check if CAPS.
    if (!found && (action.key.code == KC_CAPS)) {
      if (record->event.pressed ) {
        if (is3278) {

          if (get_keyboard_kind() == PC_XT) {
            scancode = 0x0C;
          } else {
            scancode = 0x19;
          }
        } else {
          if (get_keyboard_kind() == PC_XT) {
            scancode = 0x68;
          } else {
            scancode = 0x54;
          }
        }
        print("CAPS PRESSED\n");
        found = true;
      } else {
        if (is3278) {

          if (get_keyboard_kind() == PC_XT) {
            scancode = 0x00;
          } else {
            scancode = 0x99;
          }
        } else {

          if (get_keyboard_kind() == PC_XT) {
            scancode = 0x00;
          } else {
            scancode = 0xD4;
          }
        }
        print("CAPS RELEASED\n");
        found = true;
      }
    }


    //Check if ALT (3278 non-xt)
    if (!found && is3278 && !(get_keyboard_kind() == PC_XT) && ((action.key.code == KC_RALT) || (action.key.code == KC_LALT)) ) {
      if (record->event.pressed ) {
        scancode = 0x79;
        print("ALT PRESSED\n");
        altPressed = true;
        found = true;
      } else {
        scancode = 0xF9;
        print("ALT RELEASED\n");
        altPressed = false;
        found = true;
      }
    }



    //Check if ALT (3278 XT)
    if (!found && is3278 && (get_keyboard_kind() == PC_XT) && ((action.key.code == KC_RALT) || (action.key.code == KC_LALT)) ) {
      if (record->event.pressed ) {
        scancode = 0x16;
        print("ALT PRESSED\n");
        found = true;
      } else {
        print("ALT RELEASED\n");
        found = true;
      }
    }



    //Check if ALT (5251 XT)
    if (!found && !is3278 && (action.key.code == KC_LALT) &&  (get_keyboard_kind() == PC_XT)  ) {
      if (record->event.pressed ) {
        scancode = 0x7E;
        print("ALT PRESSED\n");
        found = true;
      } else {
        print("ALT RELEASED\n");
        found = true;
      }
    }



    //Check if LCTRL
    if (!found && ((action.key.code == KC_LCTRL)) ) {
      if (record->event.pressed ) {
        if (is3278) {

          if (get_keyboard_kind() == PC_XT) {
            scancode = 0x19;
          } else {
            scancode = 0x16;
          }


        } else {
          if (get_keyboard_kind() == PC_XT) {
            scancode = 0x54;
          } else {
            scancode = 0x7E;
          }
        }
        print("LCTRL PRESSED\n");
        found = true;
      } else {
        if (!is3278 && (get_keyboard_kind() == PC_XT) ) {
          scancode = 0xD4;
        }

        if (is3278 && (get_keyboard_kind() == PC_XT) ) {
          scancode = 0x99;
        }
        print("LCTRL RELEASED\n");
        found = true;
        }
    }



    //Check if RCTRL
    if (!found && ((action.key.code == KC_RCTRL))) {
    if (record->event.pressed ) {
      if (is3278) {
        scancode = 0x0C;
      } else {
        scancode = 0x68;
      }
      print("RCTRL PRESSED\n");
      found = true;
    } else {
      print("RCTRL RELEASED\n");
      found = true;
      }

    }


    //SIMULATE ALT in 3278 XT (XT lacks one key for that)
    if (!found && is3278 && (action.key.code == 0x55) &&  (get_keyboard_kind() == PC_XT)  ) {
      if (record->event.pressed ) {
        scancode = 0x79;
        print("ALT PRESSED\n");
        found = true;
      } else {
        print("ALT RELEASED\n");
        scancode = 0xF9;
        found = true;
      }
    }



    bool needsShift = false;
    bool needsAlt = false;



    if (!found) {

      bool enable_typematic = false;
      if (record->event.pressed) {
        xprintf("PRESSED ", action.kind.id);
        //if (is_typematic(action.key.code)) {

        if (is_typematic(action.key.code)) {
          enable_typematic=true;
        }
        //}
      } else {
        xprintf("NOT PRESSED ", action.kind.id);
        typematic_cycles = 0;
        typematic_enabled = false;
      }



      for(int row = 0; row < MATRIX_ROWS_DEST; row++)
      {
        for(int col = 0; col < MATRIX_COLS_DEST; col++)
        {

          uint8_t unimap_code = UNIMAP_NO ;
          uint8_t unimap_code_extra = UNIMAP_NO ;
          uint8_t unimap_alted_code = UNIMAP_NO ;

          //Check if 5251 or 3278 is connected

          if ( is3278 ) {
            //Get 3278 code

            unimap_alted_code = pgm_read_byte(&unimap_trans_dest_3278_alted[row][col]);

            switch (get_keyboard_kind()) {
                case PC_XT:
                    unimap_code = pgm_read_byte(&unimap_trans_dest_cs1_3278[row][col]);
                    break;
                case PC_AT:
                    unimap_code = pgm_read_byte(&unimap_trans_dest_cs2_3278[row][col]);
                    break;
                case PC_TERMINAL:
                    unimap_code = pgm_read_byte(&unimap_trans_dest_cs3_3278[row][col]);
                    unimap_code_extra = pgm_read_byte(&unimap_trans_dest_cs3_3278_extra[row][col]);
                    unimap_alted_code = pgm_read_byte(&unimap_trans_dest_cs3_3278_alted[row][col]);
                    break;
                default:
                    unimap_code = UNIMAP_NO;
                    break;
            }

          } else {
              //Get 5251 code

              switch (get_keyboard_kind()) {
                  case PC_XT:
                      unimap_code = pgm_read_byte(&unimap_trans_dest_cs1_5251[row][col]);
                      break;
                  case PC_AT:
                      unimap_code = pgm_read_byte(&unimap_trans_dest_cs2_5251[row][col]);
                      break;
                  case PC_TERMINAL:
                      unimap_code = pgm_read_byte(&unimap_trans_dest_cs3_5251[row][col]);
                      unimap_code_extra = pgm_read_byte(&unimap_trans_dest_cs3_5251_extra[row][col]);
                      break;
                  default:
                      unimap_code = UNIMAP_NO;
                      break;
              }
          }

          if (unimap_code == action.key.code || unimap_alted_code == action.key.code || unimap_code_extra == action.key.code  ) {
            //Check if key is registered for make/break (TBD)
            if (record->event.pressed ) {

              if (is3278) {
                xprintf(" TERM 3278 ");

                if ( unimap_alted_code == action.key.code ) {
                          needsAlt= true;
                        }
              } else {
                xprintf(" TERM 5251 ");
              }

              scancode = row * 8 + col;
              xprintf("TRANSLATED TO: %02X ", scancode);
              found = true;

              if (enable_typematic) {
                typematic_data = scancode;
                typematic_cycles = 500;
                typematic_enabled = true;
              }
            }
            break;
          }
        }

        if (found) {
          break;
        }
      }

    }

    print("\n");


    if (found && scancode != 0x00) {

      if (needsShift && !shiftPressed ) {
        ibm5251_send_proxy(0x59);
      }

      if (needsAlt && !altPressed ) {
        ibm5251_send_proxy(0x79);
      }

      ibm5251_send_proxy(scancode);

      if (needsAlt && !altPressed ) {
        ibm5251_send_proxy(0xF9);
      }

      if (needsShift && !shiftPressed ) {
        ibm5251_send_proxy(0xD9);
      }
    }

  }

  return true;
}
