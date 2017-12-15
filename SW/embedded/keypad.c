#include "keypad.h"
#include "common.h"
#include <avr/pgmspace.h>


#define keypad_get_rows() (PINC & 0x0f)

/**
 * @brief Keypad layout
 * 
 */
static const unsigned char keypad_layout[4][3] PROGMEM =
{
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};


static char keypad_get_column(
    char column 
)
{
    unsigned char rows = keypad_get_rows();
    char pos;
    
    for(pos = 0; pos < 4; pos ++)
    {
        if(0 == (0x08 & rows))
            return pgm_read_byte(&(keypad_layout[pos][column]));
        
        rows <<= 1;
    }

    return '\0';
}

/******************************************************************************/
void keypad_init(
    void
)
{
    /* port connection: columns: PB1, PB2, PD7 and rows: PC0 - PC3 */
    
    /* Make sure internal pull-ups are not disabled */
    MCUCR &= ~(1 << PUD);
    
    /* Set all keypad pins as inputs to enable pull-ups */
    DDRB &= ~((1 << DDB1));
    DDRD &= ~((1 << DDD7));
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3));
    
    /* Turn on the internal pull-ups */
    /*PORTB |= ((1 << PORTB1) | (1 << PORTB2));*/
    PORTD |= ((1 << PORTD7));
    PORTC |= ((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3));
}

/******************************************************************************/
char keypad_get(
    void
)
{
    char key = '\0';
    
    /* Select second column */
    PORTB &= ~(1 << PORTB1);
    DDRB |= (1 << DDB1);
    /* Read key */
    key = keypad_get_column(1);
    /* De-select second column */
    DDRB &= ~(1 << DDB1);
    PORTB |= (1 << PORTB1);
    
    /* Select first column */
    PORTD &= ~(1 << PORTD7);
    DDRD |= (1 << DDD7);
    /* Read key */
    key = keypad_get_column(0);
    /* De-select first column */
    DDRD &= ~(1 << DDD7);
    PORTD |= (1 << PORTD7);
    
    return key;
}