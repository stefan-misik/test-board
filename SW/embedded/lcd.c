#include "lcd.h"

/**
 * \brief Set lcd mode to DATA
 */
#define LCD_SET_D()		PORTD |= (1 << PORTD2)

/**
 * \brief Set lcd mode to COMMAND
 */
#define LCD_SET_C()		PORTD &= ~(1 << PORTD2);

/******************************************************************************/
void lcd_init(
	void
)
{
	/* Set output direction to LCD DC, SCE, LED pin */
	DDRD |= ( (1 << DDD2) | (1 << DDD4) | (1 << DDD5) );
	
	
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
