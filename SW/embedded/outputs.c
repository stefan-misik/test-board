/**
 * \file    outputs.h
 * \author  Stefan Misik
 * \brief   Driver for the outputs
 *     
 */

#include "outputs.h"
#include <avr/io.h>



/******************************************************************************/
void outputs_init(
	void
)
{
	unsigned char i;
	unsigned char tmp;
	
	/* Set output direction on pins */
	DDRD |= ((1 << DDD3) | (1 << DDD5) | (1 << DDD6));
	DDRB |= (1 << DDB0);
	
	/* Disable timer 0 interrupts */
	TIMSK0 &= ~((1 << OCIE0B) | (1 << OCIE0A) | (1 << TOIE0));
	
	/* LED = OC0A, BL = OC0B */
	/* Configure OC0A to non-inverting mode, OC0B to inverting mode */
	tmp = TCCR0A;
	tmp |= ((1 << COM0A1) | (1 << COM0B1) | (1 << COM0B0));
	tmp &= ~((1 << COM0A0));
	/* Configure mode 3 = Fast PWM */
	tmp |= ((1 << WGM01) | (1 << WGM00));
	TCCR0A = tmp;
	tmp = TCCR0B;
	tmp &= ~((1 << FOC0A) | (1 << FOC0B) | (1 << WGM02));
	/* Select clock source */
	tmp &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	tmp |= ((1 << CS02) | (0 << CS01) | (0 << CS00)); /* clkio / 256 */
	TCCR0B = tmp;
	
	/* Turn off all outputs */
	for(i = OUTPUT_LED; i <= OUTPUT_2; i ++)
	{
		outputs_set(i, 0);
	}
}

/******************************************************************************/
void outputs_set(
	outputs_e out,
	unsigned char value
)
{
	switch(out)
	{
		case OUTPUT_LED:
		OCR0A = (255 - value);
		break;
		
		case OUTPUT_BL:
		OCR0B = (255 - value);
		break;
		
		case OUTPUT_1:
		value ? (PORTB |= (1 << PORTB0)) : (PORTB &= ~(1 << PORTB0));
		break;
		
		case OUTPUT_2:
		value ? (PORTD |= (1 << PORTD3)) : (PORTD &= ~(1 << PORTD3));
		break;
	}
}

/******************************************************************************/
unsigned char outputs_get(
	outputs_e out
)
{
	char ret = 0;
	switch(out)
	{
		case OUTPUT_LED:
		ret = (255 - OCR0A);
		break;
		
		case OUTPUT_BL:
		ret = (255 - OCR0B);
		break;
		
		case OUTPUT_1:
		ret = ((PORTB >> PORTB0) & 0x01);		
		break;
		
		case OUTPUT_2:
		ret = ((PORTD >> PORTD3) & 0x01);
		break;
	}
	
	return ret;
}