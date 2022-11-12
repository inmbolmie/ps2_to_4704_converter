/*
Copyright 2010,2011,2012,2013 Jun WAKO <wakojun@gmail.com>
*/
#include <stdbool.h>
#include <util/delay.h>
#include "debug.h"
#include "ring_buffer.h"
#include "ibm5251_reversed.h"


#define WAIT(stat, us, err) do { \
    if (!wait_##stat(us)) { \
        ibm5251_error = err; \
        goto ERROR; \
    } \
} while (0)


uint8_t ibm5251_error = 0;


void ibm5251_init(void)
{
    //Init clicker port to input disable pullup
    CLICKER_DDR  &= ~(1<<CLICKER_BIT);
    CLICKER_PORT  &= ~(1<<CLICKER_BIT);

    //Init keyboard type identification ports to input disable pullup
    TERMINAL_TYPE_5251_DDR  &= ~(1<<TERMINAL_TYPE_5251_BIT);
    TERMINAL_TYPE_5251_PORT  &= ~(1<<TERMINAL_TYPE_5251_BIT);
    TERMINAL_TYPE_3278_DDR  &= ~(1<<TERMINAL_TYPE_3278_BIT);
    TERMINAL_TYPE_3278_PORT  &= ~(1<<TERMINAL_TYPE_3278_BIT);

    //init
    CLICKER_INT_INIT();
    CLICKER_INT_ON();
    sei();

    //Configure strobe ports
    strobe_configure();

    //init and configure data ports
    bool is3278 = TERMINAL_TYPE_3278_PIN & (1<<TERMINAL_TYPE_3278_BIT);
    idle(is3278);
}


/*
Keyboard to Host
*/
uint8_t ibm5251_send(uint8_t data)
{

    xprintf("Sending :%02X\n", data);
    ibm5251_error = 0;

    bool is3278 = TERMINAL_TYPE_3278_PIN & (1<<TERMINAL_TYPE_3278_BIT);

    /* Send */

    set_data(data, is3278);

    _delay_us(1);

    strobe_lo();

    _delay_us(1);

    WAIT(strobe_hi, 1000, 0x30);

    /* End */
    strobe_hi();

    _delay_us(1);

    WAIT(strobe_lo, 1000, 0x30);

    _delay_us(1);

    idle(is3278);

    return 0;
ERROR:
    idle(is3278);
    if (ibm5251_error >= 0x30) {
        xprintf("S:%02X ", ibm5251_error);
    }
    return -1;
}
