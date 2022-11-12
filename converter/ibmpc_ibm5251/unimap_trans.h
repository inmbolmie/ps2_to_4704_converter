/*
Copyright 2016 Jun Wako <wakojun@gmail.com>

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
#ifndef UNIMAP_TRNAS_DEST_H
#define UNIMAP_TRNAS_DEST_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"
#include "config.h"


extern const uint8_t PROGMEM unimap_trans_dest_cs1_5251[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs2_5251[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs3_5251[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs3_5251_extra[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;

extern const uint8_t PROGMEM unimap_trans_dest_3278_alted[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs1_3278[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs2_3278[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs3_3278[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs3_3278_extra[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;
extern const uint8_t PROGMEM unimap_trans_dest_cs3_3278_alted[MATRIX_ROWS_DEST][MATRIX_COLS_DEST] ;




extern const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS];

#endif
