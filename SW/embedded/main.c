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
#include "temperature.h"

 
 
int main(void)
{
	int temp;
	char pos = 0, i;
	outputs_init();
	lcd_init();
	temperature_init();
	
	stdout = &lcd_sout;
	
	lcd_set_pos(0, 0);
	lcd_bold(1);
	printf(" Temp Meter");
	
	outputs_set(OUTPUT_BL, 255);
	
	_delay_ms(2000);
	lcd_draw_buffer();	
	outputs_set(OUTPUT_BL, 70);
	
	while(1)
	{
		lcd_set_pos(0, 1);
		lcd_bold(0);
		temp = temperature_get();
		printf("Temp: %i.%i 'C",  temp >> 1, (temp & 0x0001) * 5);
		
		
		for(i = 16; i < 48; i++)
		{
			lcd_set_point(pos, i, 0);
		}
		lcd_set_point(pos ++, 90 - temp, 1);
		if(pos == 84)
		{
			pos = 0;
		}
		
		outputs_set(OUTPUT_LED, ((char)(temp)) << 4);
		
		lcd_draw_buffer();	
		
		_delay_ms(500);
	}	
}