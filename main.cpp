#include <avr/io.h>
#include <util/delay.h>
#include <lcd.h>


#define KEY_PRT 	PORTD
#define KEY_DDR		DDRD
#define KEY_PIN		PIND
#define DEBOUNCE_DELAY 10

unsigned char keypad[4][4] = {
        {'7','8','9','/'},
        {'4','5','6','*'},
        {'1','2','3','-'},
        {' ','0','=','+'}
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


void blink(){
    PORTB ^= (1 << PB0);
    _delay_ms(DEBOUNCE_DELAY);
}

int main() {
    DDRB = 0b00000001;

    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_set_contrast(0x00);
    lcd_gotoxy(4,1);
    lcd_puts("AVR CALC");
    lcd_charMode(DOUBLESIZE);

    lcd_gotoxy(0,2);
    lcd_charMode(NORMALSIZE);

    char led_string[20];
    char num_buffer[100];
    int buf_pos = 0;

    while(1){
        char key;
        blink();
        key = get_key();
        num_buffer[buf_pos]=key;
        buf_pos++;
        sprintf(led_string,"%c",key);
        lcd_puts(led_string);
        if(key=='+'){
            lcd_gotoxy(0,2);
        }
        asm("NOP");
    }
    return 0;
}
