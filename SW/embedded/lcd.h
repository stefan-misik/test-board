/**
 * \file    lcd.h
 * \author  Stefan Misik
 * \brief   Driver for the LCD
 *     
 */

#ifndef LCD_H_
#define LCD_H_

#include <stddef.h>

/**
 * \brief Determine whatever to use or not the local display buffer
 */
#define LCD_USE_BUFFER 1

/**
 * \brief Will cause whole lcd buffer to be drawn on lcd
 */
#define lcd_draw_buffer()	lcd_invalidate_rect(0, 0, 83, 47)

/**
 * \brief LCD's extended configuration options
 */
typedef enum
{
	LCD_TC = 0,		/** < Temperature coeficient */
	LCD_BS,			/** < Bias system */
	LCD_VOP			/** < Vop - contrast */
} lcd_ex_e;


/**
 * \brief Output stream to LCD
 */
extern FILE lcd_sout;

/**
 * \brief Initialization of LCD
 */
void lcd_init(
	void
);

/**
 * \brief Function to set LCD's extended configuration options
 * 
 * \param[in] what	Extended configuration to change
 * \param[in] val	New value of the selected configuration 
 */
void lcd_set_ex(
	lcd_ex_e what,
	unsigned char val
);

/**
 * \brief Set cursor position
 *
 * \param[in] col,row	Coordinates of the new position
 */
void lcd_set_pos(
	unsigned char col,
	unsigned char row
);

/**
 * \brief Draw a row of graphics in current position
 *
 * \param[in] data		Pointer to graphic data
 * \param[in] length	Length of graphic data pointed by data
 */
void lcd_put(
	const unsigned char * data,
	unsigned int length
);


#if LCD_USE_BUFFER == 1

/**
 * \brief Array of currently buffered graphic data 
 */
extern unsigned char lcd_buffer[504];

/**
 * \brief Causes graphic buffer to by written onto the LCD
 *
 * \param[in] left,top,right,bottom	Coordinates of rectangle to be redrawn
 */
void lcd_invalidate_rect(
	unsigned char left,
	unsigned char top,
	unsigned char right,
	unsigned char bottom
);

/**
 * \brief Sets the status of the pixel on the LCD
 *
 * \param[in] x,i	Coordinates of pixel in pixels
 * \param[in] state	New state of the selected pixel
 *
 * \attention	The lcd_draw_buffer must be called for changes
 *				to be visible
 */
void lcd_set_point(
	unsigned char x,
	unsigned char y,
	char state
);


/**
 * \brief Gets the status of the pixel on the LCD
 *
 * \param[in] x,i Coordinates of pixel in pixels
 *
 * \return	State of the selected pixel
 */
char lcd_get_point(
	unsigned char x,
	unsigned char y
);

#endif

/**
 * \brief	Draws single character on the LCD, starting from the 
 *			current position
 *
 * \param[in] c	Character to be drawn
 */
int lcd_putchar(
	int c
);

/**
 * \brief Sets the bold font flag
 *
 * \param[in] b	0 - normal texte, any other value = bold
 */
void lcd_bold(
	char b
);

#endif /* LCD_H_ */