/*
Copyright 2010,2011,2012,2013 Jun WAKO <wakojun@gmail.com>
*/
#include <stdbool.h>
#include <util/delay.h>
#include "debug.h"
#include "ring_buffer.h"
#include "ibm4704.h"


#define WAIT(stat, us, err) do { \
    if (!wait_##stat(us)) { \
        ibm4704_error = err; \
        goto ERROR; \
    } \
} while (0)


uint8_t ibm4704_error = 0;


void ibm4704_init(void)
{
    //inhibit();  // keep keyboard from sending
    IBM4704_INT_INIT();
    IBM4704_INT_ON();
    idle();
    data_lo();
    clock_hi();
}


/*
Keyboard to Host
----------------
Data bits are LSB first and Parity is odd. Clock has around 60us high and 30us low part.

        ____       __   __   __   __   __   __   __   __   __   _______
Clock       \_____/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/
             ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
Data    ____/    X____X____X____X____X____X____X____X____X____X________
            Start   0    1    2    3    4    5    6    7    P  Stop

Start bit:  can be long as 300-350us.
Inhibit:    Pull Data line down to inhibit keyboard to send.
Timing:     Host reads bit while Clock is hi.(rising edge)
Stop bit:   Keyboard pulls down Data line to lo after 9th clock.
*/
uint8_t ibm4704_send(uint8_t data)
{
    bool parity = true; // odd parity
    ibm4704_error = 0;

    IBM4704_INT_OFF();

    /* Request to send */

    clock_lo();
    data_hi();

    WAIT(data_hi, 30, 0x30);

    _delay_us(300);

    for (uint8_t i = 0; i < 8; i++) {
        _delay_us(30);
        if (data&(1<<i)) {
            parity = !parity;
            data_hi();
        } else {
            data_lo();
        }
        clock_hi();
        _delay_us(60);
        clock_lo();
    }

    /* Parity bit */
    _delay_us(30);
    if (parity) { data_hi(); } else { data_lo(); }
    clock_hi();
    _delay_us(60);

    /* Stop bit */
    clock_lo();
    _delay_us(30);

    data_lo();
    clock_hi();
    _delay_us(60);
    /* End */


    IBM4704_INT_ON();
    return 0;
ERROR:
    clock_hi();
    data_lo();
    if (ibm4704_error >= 0x30) {
        xprintf("S:%02X ", ibm4704_error);
    }
    IBM4704_INT_ON();
    return -1;
}

/* wait forever to receive data */
uint8_t ibm4704_recv_response(void)
{
    while (!rbuf_has_data()) {
        _delay_ms(1);
    }
    return rbuf_dequeue();
}

uint8_t ibm4704_recv(void)
{
    if (rbuf_has_data()) {
        return rbuf_dequeue();
    } else {
        return -1;
    }
}


/*
Host to Keyboard
----------------
Data bits are LSB first and Parity is odd. Clock has around 60us high and 30us low part.

        ____        __   __   __   __   __   __   __   __   __   ________
Clock       \______/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/
            ^   ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ___
Data    ____|__/    X____X____X____X____X____X____X____X____X____X   \___
            |  Start   0    1    2    3    4    5    6    7    P   Stop
            Request by host

Start bit:  can be long as 300-350us during start up and upto 2500us while key scanning
Request:    Host pulls Clock line down to request to send a command.
Timing:     After Request keyboard pull up Data and down Clock line to low for start bit.
            After request host release Clock line once Data line becomes hi.
            Host writes a bit while Clock is hi and Keyboard reads while low.
Stop bit:   Host releases or pulls up Data line to hi after 9th clock and waits for keyboard pull down the line to lo.
*/


ISR(IBM4704_INT_VECT)
{
  IBM4704_INT_OFF();
    print("INT in\n");


    // LSB first
    static uint8_t data = 0;
    // Odd parity
    static uint8_t parity = false;

    ibm4704_error = 0;

    clock_lo();
    data_hi();

    WAIT(data_hi, 30, 0x30);

    _delay_us(300);
    clock_hi();

    for (uint8_t i = 0; i < 8; i++) {

        data >>= 1;
        _delay_us(60);
        clock_lo();
        if (data_in()) {
            data |= 0x80;
            parity = !parity;
        }
        _delay_us(30);
        clock_hi();
    }

    _delay_us(60);
    clock_lo();

    if (data_in()) {
        parity = !parity;
    }

    _delay_us(30);
    clock_hi();
    _delay_us(60);
    data_lo();

    if (!parity)
        goto ERROR;


    xprintf("R:%02X\n", data);

    goto RETURN;
ERROR:
    clock_hi();
    data_lo();
    print("ERROR\n");
    ibm4704_error = 1;
    //while (ibm4704_send(0xFE)) _delay_ms(1); // resend
    //xprintf("R:%02X%02X\n", state, data);

RETURN:
    print("INT out\n");
    IBM4704_INT_ON();
    return;
}
