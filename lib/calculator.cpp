//
// Created by alexr on 04/01/25.
//
#include <stdlib.h>
#include <avr/io.h>
#include "lcd.h"
#include "calculator.h"
#include "keyboard.h"

#define MAX_DIGITS 14

enum CalcState{
    Init,
    Lhs,
    Op,
    Rhs,
    Display
};

CalcState state = Init;
int count;
double r, a, b;
unsigned char op;

void startup(){
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_set_contrast(0x00);
    reset();
};

void run(){
    while (true){
        handle_key(get_key());
    }
}

void display_result(){
    char result_string[20];
    dtostrf(r,6,4,result_string);
    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts(result_string);
}

void reset(){
    lcd_clrscr();
    r = a = b = op = count = 0;
    state = Init;
    lcd_charMode(DOUBLESIZE);
    lcd_puts("AVR CALC");
}

void handle_key(unsigned char key){
    if(key>='0' && key <= '9'){ // Entering a digit
        unsigned char digit = key - '0';
        switch (state) {
            case Init:
                if(digit){
                    lcd_clrscr();
                    state = Lhs;
                }
            case Lhs:
                if (count==MAX_DIGITS) return;
                a = a*10 + digit;
                if(a>0) {
                    lcd_putc(key);
                    count++;
                }
                break;
            case Op:
                if(digit) state=Rhs;
            case Rhs:
                b = b*10 + digit;
                if(b > 0){
                    lcd_putc(key);
                    count++;
                }
                break;
            case Display:
                if(digit){
                    lcd_clrscr();
                    a=b=op=0;
                    handle_key(key);
                    return;
                }
                break;
        }
    } else { // Entering an operator
        switch (key){
            case '/':
            case '*':
            case '+':
            case '-':
                switch (state) {
                    case Op:
                        lcd_clrscr();
                    case Lhs:
                        lcd_putc(key);
                        count++;
                        state=Op;
                        break;
                    case Rhs:
                        calculate(a,b,op);
                        display_result();
                        a=r;
                        b=0;
                        state=Lhs;
                    case Display:
                        lcd_clrscr();
                        break;
                }
                op = key;
                break;
            case '=':
                if(state==Rhs){
                    double result = calculate(a,op,b);
                    display_result();
                    a=result;
                    b=0;
                    state=Op;
                }
                break;
            case 'C':
                if(state!=Init) reset();
                break;
        }
    }
}

double calculate(long double lhs, char operand, long double rhs){
    switch (operand) {
        case '+':
            return r = lhs + rhs;
        case '-':
            return r = lhs - rhs;
        case '*':
            return r = lhs * rhs;
        case '/':
            return r = lhs / rhs;
    }
    return r = lhs;
}

void calculator(){
    startup();
    run();
}