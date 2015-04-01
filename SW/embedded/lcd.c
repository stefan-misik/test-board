#define F_CPU 20000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
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
 * \brief Macro to calculate display position from coordinates
 */
#define lcd_make_pos(col, row)	((col) + (84 * (row)))

/**
 * \brief Position on the display 
 */
static unsigned int lcd_pos;


#if LCD_USE_BUFFER == 1

/**
 * \brief Image on the display
 */
unsigned char lcd_buffer[504];

#endif





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
static void lcd_set_pos_remote
(
	unsigned char col,
	unsigned char row
)
{
	/* Enable serial interface */
	lcd_set_sce(0);
	/* Set command mode */
	lcd_set_c();
	/* lcd_transmit(0x20); */ /* Basic commands */
	lcd_transmit(0x40 | (row & 0x07)); /* Set Y */
	lcd_transmit(0x80 | (col & 0x7F)); /* Set X */
	
	/* Release serial interface */
	lcd_set_sce(1);
}

/******************************************************************************/
void lcd_init(
	void
)
{
	/* Set output direction to LCD DC, SCE, LED pin */
	DDRD |= ( (1 << DDD2) | (1 << DDD4) | (1 << DDD5) );
	/* Set SS pin as output */
	DDRB |= (1 << DDB2);
	
	/* Set MOSI and SCK output, all others input */
	DDRB |= ( (1 << DDB3) | (1 << DDB5) );
	/* DDRB &= ~( (1 << DDB4) ); */
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = ( (1 << SPE) | (1 << MSTR) | (1 << SPR0) );
	
	/* Enable backglight */
	lcd_set_bl(1);
	
	/* Reset serial interface */
	lcd_set_sce(1);
	
	lcd_set_ex(LCD_VOP, 55);	/* Set LCD Vop (Contrast) */	
	lcd_set_ex(LCD_TC, 0);		/* Set temp coefficient */	
	lcd_set_ex(LCD_BS, 4);		/* LCD bias mode 1:48 */	
	
	/* Enable LCD */
	lcd_set_sce(0);
		
	/* Switch to COMMAND mode */
	lcd_set_c();
	lcd_transmit(0x09); /* All display segments on */
	_delay_ms(1000);
	lcd_transmit(0x0C); /* Normal operation */
	
	lcd_set_sce(1);
	
	lcd_set_pos(0, 0);
	
	#if LCD_USE_BUFFER == 1
	do 
	{
		int i;
		for(i = 0; i < 504; i ++)
		{
			lcd_buffer[i] = 0;
		}		
	} while (0);		
	#endif
}

/******************************************************************************/
void lcd_set_ex(
	lcd_ex_e what,
	unsigned char val
)
{
	static const unsigned char bases[] PROGMEM = {0x04, 0x10, 0x80};
	static const unsigned char masks[] PROGMEM = {0x03, 0x07, 0x7F};
		
	if (what <= LCD_VOP)
	{	
		/* Enable serial interface */
		lcd_set_sce(0);
		/* Set command mode */
		lcd_set_c();
	
		lcd_transmit(0x21); /* Extended commands */
		/* Write command */
		lcd_transmit((pgm_read_byte(bases + what)) | 
			(val & (pgm_read_byte(masks + what))));
		lcd_transmit(0x20); /* Basic commands */
		
		/* Release serial interface */
		lcd_set_sce(1);
	}
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

/******************************************************************************/
void lcd_put(
	unsigned char * data,
	unsigned int length
)
{	
	unsigned char tmp;
	
	/* Enable serial interface */
	lcd_set_sce(0);
	/* Set data mode */
	lcd_set_d();
	while(length > 0 && lcd_pos < 504)
	{
		tmp = *(data++);
		
		/* Store data in LCD buffer, if enabled */
		#if LCD_USE_BUFFER == 1
		lcd_buffer[lcd_pos] = tmp;		
		#endif			
		
		/* Write on display */
		lcd_transmit(tmp);
		length --;
		lcd_pos ++;
	}
	
	/* Release serial interface */
	lcd_set_sce(1);
}

/******************************************************************************/
void lcd_set_pos(
	unsigned char col,
	unsigned char row
)
{
	col %= 84;
	row %= 6;
	
	/* Set position on LCD */
	lcd_set_pos_remote(col, row);	
	
	/* Store local position information */	
	lcd_pos = lcd_make_pos(col, row);	
}

#if LCD_USE_BUFFER == 1

/******************************************************************************/
void lcd_draw_buffer(
	void
)
{
	lcd_set_pos(0, 0);
	lcd_put(lcd_buffer, 504);
}

/******************************************************************************/
void lcd_set_point(
	unsigned char x,
	unsigned char y,
	char state
)
{
	if(x < 84 && y < 48)
	{
		if(state)
			lcd_buffer[lcd_make_pos(x, (y >> 3))] |= (1 << (y & 0x07));
		else
			lcd_buffer[lcd_make_pos(x, (y >> 3))] &= ~(1 << (y & 0x07));
			
	}	
}

#endif
