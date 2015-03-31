#include <avr/io.h>
#include "lcd.h"


/**
 * \brief Set lcd mode to DATA
 */
#define LCD_SET_D()			PORTD |= (1 << PORTD2)

/**
 * \brief Set lcd mode to COMMAND
 */
#define LCD_SET_C()			PORTD &= ~(1 << PORTD2)

/**
 * \brief Set lcd SCE
 */
#define LCD_SET_SCE(val)	(val) ? (PORTD |= (1 << PORTD4)) : (PORTD &= ~(1 << PORTD4))




/******************************************************************************/
static char lcd_transmit(
	char data
)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while( !(SPSR & (1 << SPIF)) )
		;
		
	return SPDR;
}

/******************************************************************************/
void lcd_init(
	void
)
{
	/* Set output direction to LCD DC, SCE, LED pin */
	DDRD |= ( (1 << DDD2) | (1 << DDD4) | (1 << DDD5) );
	
	/* Set MOSI and SCK output, all others input */
	DDRB |= ( (1 << DDB3) | (1 << DDB5) );
	DDRB &= ~( (1 << DDB4) );
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = ( (1 << SPE) | (1 << MSTR) | (1 << SPR0) );
	
	/* Enable LCD */
	LCD_SET_SCE(1);
}

/******************************************************************************/
void lcd_set_bl(
	char bl
)
{
	if(bl)
	{
		PORTD |= (1 << PORTD5);
	}
	else
	{
		PORTD &= ~(1 << PORTD5);
	}
}
