/*
Copyright 2014 Jun WAKO <wakojun@gmail.com>
*/
#ifndef IBM5251_H
#define IBM5251_H


void ibm5251_init(void);
uint8_t ibm5251_send(uint8_t data);

/*pin names are: STROBE, STROBE_DELAY, BREAK0, DATA1---DATA7*/
/*DDR is I/O (0/1), PORT is low/high (0/1) or disable/enable pullup (0/1) PIN is for read*/

/*--------------------------------------------------------------------
 * static functions
 *------------------------------------------------------------------*/
static inline void strobe_lo(void)
{
    /* output 0*/
    IBM5251_STROBE_DDR  |=  (1<<IBM5251_STROBE_BIT);
    IBM5251_STROBE_PORT &= ~(1<<IBM5251_STROBE_BIT);
}

static inline void strobe_hi(void)
{

  /* output 1*/
  IBM5251_STROBE_DDR  |=  (1<<IBM5251_STROBE_BIT);
  IBM5251_STROBE_PORT |=  (1<<IBM5251_STROBE_BIT);


}

static inline void set_data(uint8_t data, bool is3278)
{


  if (is3278) {
    //Data bits are reversed and polarity inverted

    IBM5251_DATA7_DDR  |=  (1<<IBM5251_DATA7_BIT);
    if (!(data&(1<<6))) {
        IBM5251_DATA7_PORT &= ~(1<<IBM5251_DATA7_BIT);
    } else {
        IBM5251_DATA7_PORT |=  (1<<IBM5251_DATA7_BIT);
    }

    IBM5251_DATA6_DDR  |=  (1<<IBM5251_DATA6_BIT);
    if (!(data&(1<<5))) {
        IBM5251_DATA6_PORT &= ~(1<<IBM5251_DATA6_BIT);
    } else {
        IBM5251_DATA6_PORT |=  (1<<IBM5251_DATA6_BIT);
    }

    IBM5251_DATA5_DDR  |=  (1<<IBM5251_DATA5_BIT);
    if (!(data&(1<<4))) {
        IBM5251_DATA5_PORT &= ~(1<<IBM5251_DATA5_BIT);
    } else {
        IBM5251_DATA5_PORT |=  (1<<IBM5251_DATA5_BIT);
    }

    IBM5251_DATA4_DDR  |=  (1<<IBM5251_DATA4_BIT);
    if (!(data&(1<<3))) {
        IBM5251_DATA4_PORT &= ~(1<<IBM5251_DATA4_BIT);
    } else {
        IBM5251_DATA4_PORT |=  (1<<IBM5251_DATA4_BIT);
    }

    IBM5251_DATA3_DDR  |=  (1<<IBM5251_DATA3_BIT);
    if (!(data&(1<<2))) {
        IBM5251_DATA3_PORT &= ~(1<<IBM5251_DATA3_BIT);
    } else {
        IBM5251_DATA3_PORT |=  (1<<IBM5251_DATA3_BIT);
    }

    IBM5251_DATA2_DDR  |=  (1<<IBM5251_DATA2_BIT);
    if (!(data&(1<<1))) {
        IBM5251_DATA2_PORT &= ~(1<<IBM5251_DATA2_BIT);
    } else {
        IBM5251_DATA2_PORT |=  (1<<IBM5251_DATA2_BIT);
    }

    IBM5251_DATA1_DDR  |=  (1<<IBM5251_DATA1_BIT);
    if (!(data&(1<<0))) {
        IBM5251_DATA1_PORT &= ~(1<<IBM5251_DATA1_BIT);
    } else {
        IBM5251_DATA1_PORT |=  (1<<IBM5251_DATA1_BIT);
    }


    IBM5251_BREAK0_DDR  |=  (1<<IBM5251_BREAK0_BIT);
    if (!(data&(1<<7))) {
        IBM5251_BREAK0_PORT &= ~(1<<IBM5251_BREAK0_BIT);
    } else {
        IBM5251_BREAK0_PORT |=  (1<<IBM5251_BREAK0_BIT);
    }

  } else {
    //5251
    IBM5251_DATA7_DDR  |=  (1<<IBM5251_DATA7_BIT);
    if (data&(1<<0)) {
        IBM5251_DATA7_PORT &= ~(1<<IBM5251_DATA7_BIT);
    } else {
        IBM5251_DATA7_PORT |=  (1<<IBM5251_DATA7_BIT);
    }

    IBM5251_DATA6_DDR  |=  (1<<IBM5251_DATA6_BIT);
    if (data&(1<<1)) {
        IBM5251_DATA6_PORT &= ~(1<<IBM5251_DATA6_BIT);
    } else {
        IBM5251_DATA6_PORT |=  (1<<IBM5251_DATA6_BIT);
    }

    IBM5251_DATA5_DDR  |=  (1<<IBM5251_DATA5_BIT);
    if (data&(1<<2)) {
        IBM5251_DATA5_PORT &= ~(1<<IBM5251_DATA5_BIT);
    } else {
        IBM5251_DATA5_PORT |=  (1<<IBM5251_DATA5_BIT);
    }

    IBM5251_DATA4_DDR  |=  (1<<IBM5251_DATA4_BIT);
    if (data&(1<<3)) {
        IBM5251_DATA4_PORT &= ~(1<<IBM5251_DATA4_BIT);
    } else {
        IBM5251_DATA4_PORT |=  (1<<IBM5251_DATA4_BIT);
    }

    IBM5251_DATA3_DDR  |=  (1<<IBM5251_DATA3_BIT);
    if (data&(1<<4)) {
        IBM5251_DATA3_PORT &= ~(1<<IBM5251_DATA3_BIT);
    } else {
        IBM5251_DATA3_PORT |=  (1<<IBM5251_DATA3_BIT);
    }

    IBM5251_DATA2_DDR  |=  (1<<IBM5251_DATA2_BIT);
    if (data&(1<<5)) {
        IBM5251_DATA2_PORT &= ~(1<<IBM5251_DATA2_BIT);
    } else {
        IBM5251_DATA2_PORT |=  (1<<IBM5251_DATA2_BIT);
    }

    IBM5251_DATA1_DDR  |=  (1<<IBM5251_DATA1_BIT);
    if (data&(1<<6)) {
        IBM5251_DATA1_PORT &= ~(1<<IBM5251_DATA1_BIT);
    } else {
        IBM5251_DATA1_PORT |=  (1<<IBM5251_DATA1_BIT);
    }


    IBM5251_BREAK0_DDR  |=  (1<<IBM5251_BREAK0_BIT);
    if (data&(1<<7)) {
        IBM5251_BREAK0_PORT &= ~(1<<IBM5251_BREAK0_BIT);
    } else {
        IBM5251_BREAK0_PORT |=  (1<<IBM5251_BREAK0_BIT);
    }
  }
}

static inline void strobe_configure(void)
{
    /* strobe delay input disable pullup*/
    IBM5251_STROBE_DELAY_DDR  &= ~(1<<IBM5251_STROBE_DELAY_BIT);
    IBM5251_STROBE_DELAY_PORT &= ~(1<<IBM5251_STROBE_DELAY_BIT);

    /* strobe output to high*/
    strobe_hi();
}

static inline bool strobe_in(void)
{
    return IBM5251_STROBE_DELAY_PIN&(1<<IBM5251_STROBE_DELAY_BIT);
}

static inline uint16_t wait_strobe_lo(uint16_t us)
{
    while (strobe_in() && us)  { asm(""); _delay_us(1); us--; }
    return us;
}

static inline uint16_t wait_strobe_hi(uint16_t us)
{
    while (!strobe_in() && us)  { asm(""); _delay_us(1); us--; }
    return us;
}

/* idle state that device can send */
static inline void idle(bool is3278)
{
  if (is3278) {
    set_data(0xFF, is3278);
  } else {
    set_data(0x00, is3278);
  }
  strobe_hi();

}

/* inhibit device to send
 * keyboard checks Data line on start bit(Data:hi) and it stops sending if Data line is low.
 */
static inline void inhibit(void)
{
 return;
}

#endif
