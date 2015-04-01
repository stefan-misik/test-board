#ifndef LCD_H_
#define LCD_H_

#define LCD_USE_BUFFER 1

typedef enum
{
	LCD_TC = 0,		/* Temperature coeficient */
	LCD_BS,			/* Bias system */
	LCD_VOP			/* Vop - contrast */
} lcd_ex_e;

void lcd_init(
	void
);

void lcd_set_ex(
	lcd_ex_e what,
	unsigned char val
);

void lcd_set_bl(
	char bl
);

void lcd_set_pos(
	unsigned char col,
	unsigned char row
);

void lcd_put(
	unsigned char * data,
	unsigned int length
);

#if LCD_USE_BUFFER == 1

extern unsigned char lcd_buffer[504];

void lcd_draw_buffer(
	void
);

void lcd_set_point(
	unsigned char x,
	unsigned char y,
	char state
);

#endif

#endif /* LCD_H_ */