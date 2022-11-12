#ifndef IBMPC_INTERFACE_H
#define IBMPC_INTERFACE_H


extern "C" void matrix_init_ibmpc(void);
extern "C" uint8_t matrix_scan_ibmpc(void);
extern "C" matrix_row_t matrix_get_row_ibmpc(uint8_t row);
extern "C" void matrix_clear_ibmpc(void);
extern "C" keyboard_kind_t get_keyboard_kind(void);

#endif
