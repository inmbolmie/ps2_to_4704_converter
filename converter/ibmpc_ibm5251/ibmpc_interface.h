#ifndef IBMPC_INTERFACE_H
#define IBMPC_INTERFACE_H

typedef enum { NONE, PC_XT, PC_AT, PC_TERMINAL, PC_MOUSE } keyboard_kind_t;

void matrix_init_ibmpc(void);
uint8_t matrix_scan_ibmpc(void);
matrix_row_t matrix_get_row_ibmpc(uint8_t row);
void matrix_clear_ibmpc(void);
keyboard_kind_t get_keyboard_kind(void);


#endif
