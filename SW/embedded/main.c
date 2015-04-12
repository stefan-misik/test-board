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
#include "timer.h"


char fix_to_decimal(
	char in
)
{
	char out = 0;
	char cur = 5;
	for(; in != 0; in <<= 1)
	{
		out *= 10;
		if(0x80 & in)
		{
			out += cur;
		}
		cur *= 5;		
	}
	
	return out;
}
 
int main(void)
{
	int temp, t_max, t_min;
	unsigned int i;
	unsigned char j;
	
	t_max = 0x8000;
	t_min = 0x7FFF;
	
	timer_init();
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
	
	for(i = 255; i > 0; i--)
	{
		outputs_set(OUTPUT_BL, i);
		_delay_ms(5);
	}
	
	outputs_set(OUTPUT_BL, 128);
	
	i = 0;
	
	/*while(1)
	{
		long_time_t t;
		lcd_set_pos(0, 1);
		lcd_bold(0);
		
		timer_get_long(&t);
		
		printf("%lu", (unsigned long)t);
	}*/
	
	while(1)
	{
		temp = temperature_get();
		lcd_set_pos(0, 1);
		lcd_bold(0);
		
		printf("Temp: %i.%i'C ",  temp >> 2, fix_to_decimal((temp & 0x0003) << 6));
		
				
		if(temp > t_max)
		{
			t_max = temp;
		}
		
		if(temp < t_min)
		{
			t_min = temp;
		}
		
		if(10 == i)
		{		
			t_max = 140 - t_max;
			t_min = 140 - t_min;
			
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