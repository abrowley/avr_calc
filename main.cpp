#include <avr/io.h>
#include <util/delay.h>


void blink(){
    PORTB ^= (1 << PB0);
    _delay_ms(1000);
}

int main() {
    DDRB |= (1 << PB0);
    while(1){
        blink();
    }
    return 0;
}
