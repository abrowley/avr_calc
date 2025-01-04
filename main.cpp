#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "keyboard.h"
#include "blink.h"
#include "calculator.h"

int main() {
    calculator();
    return 0;
}
