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
#include "keypad.h"


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
	char key;
    
    t_max = 0x8000;
    t_min = 0x7FFF;
    
    timer_init();
    outputs_init();
    lcd_init();
    temperature_init();
    keypad_init();
    
    temperature_update();

    outputs_set(OUTPUT_BL, 255);
	while (1)
	{
		key = keypad_get();

		if ('\0' != key)
		{
    		lcd_set_pos(0, 0);
			printf("%c", key);
		}
    	lcd_draw_buffer();    
        _delay_ms(100);
	}
    
    
    
    //lcd_set_pos(0, 0);
    lcd_bold(1);
    printf(" Temp Meter");
    
    lcd_draw_buffer();    
    
    for(i = 0; i < 256; i++)
    {
        outputs_set(OUTPUT_LED, 255-i);
        outputs_set(OUTPUT_BL, i);
        _delay_ms(5);
    }
    
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
        temperature_update();        
        temp = temperature_get();
        
        lcd_set_pos(0, 1);
        lcd_bold(0);
        
        printf("Temp: %i.%i" "\xf8" "C ",  temp >> 2, fix_to_decimal((temp & 0x0003) << 6));
        
                
        if(temp > t_max)
        {
            t_max = temp;
        }
        
        if(temp < t_min)
        {
            t_min = temp;
        }
        
        if(1 == i)
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
        
        lcd_draw_buffer();
        i ++;
        _delay_ms(500);
    }    
}
