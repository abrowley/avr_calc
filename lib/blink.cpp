//
// Created by alexr on 15/07/23.
//

#include <util/delay.h>
#include <avr/io.h>
#include "blink.h"
#include "constants.h"

void blink(void){
    PORTB ^= (1 << PB0);
    _delay_ms(DEBOUNCE_DELAY);
}