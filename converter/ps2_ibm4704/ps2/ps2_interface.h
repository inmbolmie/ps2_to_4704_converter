#ifndef PS2_INTERFACE_H
#define PS2_INTERFACE_H

void led_set_ps2(uint8_t usb_led);

void matrix_init_ps2(void);
uint8_t matrix_scan_ps2(void);
uint8_t matrix_get_row_ps2(uint8_t row);
void matrix_clear_ps2(void);

#endif
