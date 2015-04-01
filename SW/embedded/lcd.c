#define F_CPU 20000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "lcd.h"



/**
 * \brief Set lcd mode to DATA
 */
#define lcd_set_d()			PORTD |= (1 << PORTD2)

/**
 * \brief Set lcd mode to COMMAND
 */
#define lcd_set_c()			PORTD &= ~(1 << PORTD2)

/**
 * \brief Set lcd SCE
 */
#define lcd_set_sce(val)	(val) ? (PORTD |= (1 << PORTD4)) : (PORTD &= ~(1 << PORTD4))

/**
 * \brief Image on the display
 */
/* static unsigned char lcd_data[504]; */




/******************************************************************************/
static void lcd_transmit(
	char data
)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while( !(SPSR & (1 << SPIF)) )
		;	
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
	/* DDRB &= ~( (1 << DDB4) ); */
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = ( (1 << SPE) | (1 << MSTR) | (1 << SPR0) );
	
		
	/* Enable LCD */
	lcd_set_sce(1);
	//_delay_ms(200);
	lcd_set_sce(0);
	//_delay_ms(200);
	/* Switch to COMMAND mode */
	lcd_set_c();
	//_delay_ms(200);
	lcd_transmit(0x21);
	lcd_transmit(0xB1);
	lcd_transmit(0x04);
	lcd_transmit(0x14);
	lcd_transmit(0x20);
	lcd_transmit(0x0C);
	
	//_delay_ms(200);
	lcd_set_d();	
	//lcd_transmit(0xaa);
	
	//lcd_transmit(0x09);
	
	lcd_set_sce(1);
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
