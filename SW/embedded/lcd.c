/**
 * \file    lcd.c
 * \author  Stefan Misik
 * \brief   Driver for the LCD
 *     
 */

#define F_CPU 20000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
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

FILE lcd_sout = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

/**
 * \brief ASCII characters
 */
static const unsigned char lcd_ascii[][5] PROGMEM =
{
	{0x00, 0x00, 0x00, 0x00, 0x00}, /* 20 */
	{0x00, 0x00, 0x5f, 0x00, 0x00}, /* 21 ! */
	{0x00, 0x07, 0x00, 0x07, 0x00}, /* 22 " */
	{0x14, 0x7f, 0x14, 0x7f, 0x14}, /* 23 # */
	{0x24, 0x2a, 0x7f, 0x2a, 0x12}, /* 24 $ */
	{0x23, 0x13, 0x08, 0x64, 0x62}, /* 25 % */
	{0x36, 0x49, 0x55, 0x22, 0x50}, /* 26 & */
	{0x00, 0x05, 0x03, 0x00, 0x00}, /* 27 ' */
	{0x00, 0x1c, 0x22, 0x41, 0x00}, /* 28 ( */
	{0x00, 0x41, 0x22, 0x1c, 0x00}, /* 29 ) */
	{0x14, 0x08, 0x3e, 0x08, 0x14}, /* 2a * */
	{0x08, 0x08, 0x3e, 0x08, 0x08}, /* 2b + */
	{0x00, 0x50, 0x30, 0x00, 0x00}, /* 2c , */
	{0x08, 0x08, 0x08, 0x08, 0x08}, /* 2d - */
	{0x00, 0x60, 0x60, 0x00, 0x00}, /* 2e . */
	{0x20, 0x10, 0x08, 0x04, 0x02}, /* 2f / */
	{0x3e, 0x51, 0x49, 0x45, 0x3e}, /* 30 0 */
	{0x00, 0x42, 0x7f, 0x40, 0x00}, /* 31 1 */
	{0x42, 0x61, 0x51, 0x49, 0x46}, /* 32 2 */
	{0x21, 0x41, 0x45, 0x4b, 0x31}, /* 33 3 */
	{0x18, 0x14, 0x12, 0x7f, 0x10}, /* 34 4 */
	{0x27, 0x45, 0x45, 0x45, 0x39}, /* 35 5 */
	{0x3c, 0x4a, 0x49, 0x49, 0x30}, /* 36 6 */
	{0x01, 0x71, 0x09, 0x05, 0x03}, /* 37 7 */
	{0x36, 0x49, 0x49, 0x49, 0x36}, /* 38 8 */
	{0x06, 0x49, 0x49, 0x29, 0x1e}, /* 39 9 */
	{0x00, 0x36, 0x36, 0x00, 0x00}, /* 3a : */
	{0x00, 0x56, 0x36, 0x00, 0x00}, /* 3b ; */
	{0x08, 0x14, 0x22, 0x41, 0x00}, /* 3c < */
	{0x14, 0x14, 0x14, 0x14, 0x14}, /* 3d = */
	{0x00, 0x41, 0x22, 0x14, 0x08}, /* 3e > */
	{0x02, 0x01, 0x51, 0x09, 0x06}, /* 3f ? */
	{0x32, 0x49, 0x79, 0x41, 0x3e}, /* 40 @ */
	{0x7e, 0x11, 0x11, 0x11, 0x7e}, /* 41 A */
	{0x7f, 0x49, 0x49, 0x49, 0x36}, /* 42 B */
	{0x3e, 0x41, 0x41, 0x41, 0x22}, /* 43 C */
	{0x7f, 0x41, 0x41, 0x22, 0x1c}, /* 44 D */
	{0x7f, 0x49, 0x49, 0x49, 0x41}, /* 45 E */
	{0x7f, 0x09, 0x09, 0x09, 0x01}, /* 46 F */
	{0x3e, 0x41, 0x49, 0x49, 0x7a}, /* 47 G */
	{0x7f, 0x08, 0x08, 0x08, 0x7f}, /* 48 H */
	{0x00, 0x41, 0x7f, 0x41, 0x00}, /* 49 I */
	{0x20, 0x40, 0x41, 0x3f, 0x01}, /* 4a J */
	{0x7f, 0x08, 0x14, 0x22, 0x41}, /* 4b K */
	{0x7f, 0x40, 0x40, 0x40, 0x40}, /* 4c L */
	{0x7f, 0x02, 0x0c, 0x02, 0x7f}, /* 4d M */
	{0x7f, 0x04, 0x08, 0x10, 0x7f}, /* 4e N */
	{0x3e, 0x41, 0x41, 0x41, 0x3e}, /* 4f O */
	{0x7f, 0x09, 0x09, 0x09, 0x06}, /* 50 P */
	{0x3e, 0x41, 0x51, 0x21, 0x5e}, /* 51 Q */
	{0x7f, 0x09, 0x19, 0x29, 0x46}, /* 52 R */
	{0x46, 0x49, 0x49, 0x49, 0x31}, /* 53 S */
	{0x01, 0x01, 0x7f, 0x01, 0x01}, /* 54 T */
	{0x3f, 0x40, 0x40, 0x40, 0x3f}, /* 55 U */
	{0x1f, 0x20, 0x40, 0x20, 0x1f}, /* 56 V */
	{0x3f, 0x40, 0x38, 0x40, 0x3f}, /* 57 W */
	{0x63, 0x14, 0x08, 0x14, 0x63}, /* 58 X */
	{0x07, 0x08, 0x70, 0x08, 0x07}, /* 59 Y */
	{0x61, 0x51, 0x49, 0x45, 0x43}, /* 5a Z */
	{0x00, 0x7f, 0x41, 0x41, 0x00}, /* 5b [ */
	{0x02, 0x04, 0x08, 0x10, 0x20}, /* 5c ? */
	{0x00, 0x41, 0x41, 0x7f, 0x00}, /* 5d ] */
	{0x04, 0x02, 0x01, 0x02, 0x04}, /* 5e ^ */
	{0x40, 0x40, 0x40, 0x40, 0x40}, /* 5f _ */
	{0x00, 0x01, 0x02, 0x04, 0x00}, /* 60 ` */
	{0x20, 0x54, 0x54, 0x54, 0x78}, /* 61 a */
	{0x7f, 0x48, 0x44, 0x44, 0x38}, /* 62 b */
	{0x38, 0x44, 0x44, 0x44, 0x20}, /* 63 c */
	{0x38, 0x44, 0x44, 0x48, 0x7f}, /* 64 d */
	{0x38, 0x54, 0x54, 0x54, 0x18}, /* 65 e */
	{0x08, 0x7e, 0x09, 0x01, 0x02}, /* 66 f */
	{0x0c, 0x52, 0x52, 0x52, 0x3e}, /* 67 g */
	{0x7f, 0x08, 0x04, 0x04, 0x78}, /* 68 h */
	{0x00, 0x44, 0x7d, 0x40, 0x00}, /* 69 i */
	{0x20, 0x40, 0x44, 0x3d, 0x00}, /* 6a j */
	{0x7f, 0x10, 0x28, 0x44, 0x00}, /* 6b k */
	{0x00, 0x41, 0x7f, 0x40, 0x00}, /* 6c l */
	{0x7c, 0x04, 0x18, 0x04, 0x78}, /* 6d m */
	{0x7c, 0x08, 0x04, 0x04, 0x78}, /* 6e n */
	{0x38, 0x44, 0x44, 0x44, 0x38}, /* 6f o */
	{0x7c, 0x14, 0x14, 0x14, 0x08}, /* 70 p */
	{0x08, 0x14, 0x14, 0x18, 0x7c}, /* 71 q */
	{0x7c, 0x08, 0x04, 0x04, 0x08}, /* 72 r */
	{0x48, 0x54, 0x54, 0x54, 0x20}, /* 73 s */
	{0x04, 0x3f, 0x44, 0x40, 0x20}, /* 74 t */
	{0x3c, 0x40, 0x40, 0x20, 0x7c}, /* 75 u */
	{0x1c, 0x20, 0x40, 0x20, 0x1c}, /* 76 v */
	{0x3c, 0x40, 0x30, 0x40, 0x3c}, /* 77 w */
	{0x44, 0x28, 0x10, 0x28, 0x44}, /* 78 x */
	{0x0c, 0x50, 0x50, 0x50, 0x3c}, /* 79 y */
	{0x44, 0x64, 0x54, 0x4c, 0x44}, /* 7a z */
	{0x00, 0x08, 0x36, 0x41, 0x00}, /* 7b { */
	{0x00, 0x00, 0x7f, 0x00, 0x00}, /* 7c | */
	{0x00, 0x41, 0x36, 0x08, 0x00}, /* 7d } */
	{0x10, 0x08, 0x08, 0x10, 0x08}, /* 7e ? */
	{0x78, 0x46, 0x41, 0x46, 0x78}  /* 7f ? */
};

/**
 * \brief Bold flag
 */
static char lcd_fond_bold = 0;


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
	
	lcd_set_pos(0, 0);	
	
	/* Enable LCD */
	lcd_set_sce(0);
	lcd_set_d();
	
	do 
	{
		int i;
		for(i = 0; i < 504; i ++)
		{
			lcd_transmit(0x00);
			
			#if LCD_USE_BUFFER == 1
			lcd_buffer[i] = 0;
			#endif
		}		
	} while (0);		
	
	lcd_set_pos(0, 0);	
	
	lcd_set_sce(1);
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
void lcd_put(
	const unsigned char * data,
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

/******************************************************************************/
int lcd_putchar(
	int c
)
{
	unsigned char i, last_b, current_b;
	static unsigned char tmp[7]; 
	
	/* Enable serial interface */
	lcd_set_sce(0);
	/* Set data mode */
	lcd_set_d();
	
	/* Prepare character identification */
	c -= 0x20;
	if(c < 0 || c >= sizeof(lcd_ascii) / 5)
	{
		c = 0;
	}
	
	/* Prepare character to be drawn */
	last_b = 0;
	for(i = 0; i < 5; i ++)
	{
		current_b = pgm_read_byte(&(lcd_ascii[c][i]));
		tmp[i] = current_b | last_b;
		
		if(lcd_fond_bold)
		{
			last_b = current_b;
		}
	}
	
	if(lcd_fond_bold)
	{
		tmp[i++] = last_b;
	}
	
	tmp[i++] = 0;	
	
	/* Draw character */
	lcd_put(tmp, i);
	
	
	/* Release serial interface */
	lcd_set_sce(1);
	
	return c + 0x20;
}

/******************************************************************************/
void lcd_bold(
	char b
)
{
	lcd_fond_bold = (b) ? 1 : 0;
}