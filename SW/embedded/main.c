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
	int temp, t_max, t_min;
	unsigned int i;
	unsigned char j;
	
	t_max = 0x8000;
	t_min = 0x7FFF;
	
	outputs_init();
	lcd_init();
	temperature_init();
	
	stdout = &lcd_sout;
	
	lcd_set_pos(0, 0);
	lcd_bold(1);
	printf(" Temp Meter");
	
	outputs_set(OUTPUT_BL, 255);
	
	_delay_ms(1000);
	lcd_draw_buffer();	
	outputs_set(OUTPUT_BL, 0);
	
	
	i = 0;
	
	while(1)
	{
		lcd_set_pos(0, 1);
		lcd_bold(0);
		temp = temperature_get();
		printf("Temp: %i.%i 'C",  temp >> 1, (temp & 0x0001) * 5);
		
		if(temp > t_max)
		{
			t_max = temp;
		}
		
		if(temp < t_min)
		{
			t_min = temp;
		}
		
		if(120 == i)
		{		
			t_max = 90 - t_max;
			t_min = 90 - t_min;
			
			for(i = 1; i < 84; i ++)
			{
				for (j = 16; j < 48; j ++)
				{
					lcd_set_point(i-1, j,
						lcd_get_point(i,j));
				}
			}
			
			for(j = 16; j < 48; j++)
			{
				lcd_set_point(83, j, (j >= t_max) && (j <= t_min));
			}
									
								
			i = 0;	
			t_max = 0x8000;
			t_min = 0x7FFF;		
		}	
		
		
		/*outputs_set(OUTPUT_LED, ((char)(temp)) << 4);*/
		
		lcd_draw_buffer();	
		i ++;
		_delay_ms(1000);
	}	
}