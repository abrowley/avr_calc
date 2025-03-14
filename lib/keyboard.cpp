//
// Created by alexr on 04/01/25.
//
#include <avr/io.h>
#include <util/delay.h>
#include "constants.h"

#define KEY_PRT 	PORTD
#define KEY_DDR		DDRD
#define KEY_PIN		PIND


unsigned char keypad[4][4] = {
        {'1','2','3','/'},
        {'4','5','6','*'},
        {'7','8','9','-'},
        {'C','0','=','+'}
};

unsigned char column, row;

char get_key()
{
    while(true)
    {
        KEY_DDR = 0xF0;           	/* set port direction as input-output */
        KEY_PRT = 0xFF;
        do
        {
            KEY_PRT &= 0x0F;      		/* mask PORT for column read only */
            asm("NOP");
            column = (KEY_PIN & 0x0F); 	/* read status of column */
        } while (column != 0x0F);

        do
        {
            do
            {
                _delay_ms(DEBOUNCE_DELAY);             /* 20ms key debounce time */
                column = (KEY_PIN & 0x0F); /* read status of column */
            }while(column == 0x0F);        /* check for any key press */

            _delay_ms (DEBOUNCE_DELAY);	            /* 20 ms key debounce time */
            column = (KEY_PIN & 0x0F);
        }while(column == 0x0F);

        /* now check for rows */
        KEY_PRT = 0xEF;            /* check for pressed key in 1st row */
        asm("NOP");
        column = (KEY_PIN & 0x0F);
        if(column != 0x0F)
        {
            row = 0;
            break;
        }

        KEY_PRT = 0xDF;		/* check for pressed key in 2nd row */
        asm("NOP");
        column = (KEY_PIN & 0x0F);
        if(column != 0x0F)
        {
            row = 1;
            break;
        }

        KEY_PRT = 0xBF;		/* check for pressed key in 3rd row */
        asm("NOP");
        column = (KEY_PIN & 0x0F);
        if(column != 0x0F)
        {
            row = 2;
            break;
        }

        KEY_PRT = 0x7F;		/* check for pressed key in 4th row */
        asm("NOP");
        column = (KEY_PIN & 0x0F);
        if(column != 0x0F)
        {
            row = 3;
            break;
        }
    }

    if(column == 0x0E)
        return(keypad[row][0]);
    else if(column == 0x0D)
        return(keypad[row][1]);
    else if(column == 0x0B)
        return(keypad[row][2]);
    else
        return(keypad[row][3]);
}
