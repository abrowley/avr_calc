//
// Created by alexr on 15/07/23.
//

#include <util/delay.h>
#include <avr/io.h>
#include "blink.h"

void blink() {
    PORTB ^= (1 << PB0);
}