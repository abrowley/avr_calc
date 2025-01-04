//
// Created by alexr on 04/01/25.
//

#ifndef AVR_CALC_CALCULATOR_H
#define AVR_CALC_CALCULATOR_H

void calculator();
void startup();
double calculate(long double lhs, char op, long double rhs);
void display_result();
void run();
void reset();
void handle_key(unsigned char key);

#endif //AVR_CALC_CALCULATOR_H
