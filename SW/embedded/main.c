/*
 * test_boart_1.c
 *
 * Created: 5. 2. 2015 22:22:15
 *  Author: Stefan
 */ 

#define F_CPU 20000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcd.h"
#include "outputs.h"

 
 
int main(void)
{
	outputs_init();
	lcd_init();
	
	stdout = &lcd_sout;
	
	lcd_set_pos(0, 0);
	lcd_bold(0);
	printf("abcABC,./*-+");
	
	lcd_set_pos(0, 1);
	lcd_bold(10);
	printf("abcABC,./*-+");
	
	_delay_ms(2000);
	lcd_draw_buffer();	
	lcd_set_bl(0);
	
	while(1)
	{
		
	}	
}