#include "outputs.h"
#include <avr/io.h>



/******************************************************************************/
void outputs_init(
	void
)
{
	DDRD |= ((1 << DDD3) | (1 << DDD6));
	DDRB |= (1 << DDB0);
}

/******************************************************************************/
void outputs_set(
	outputs_e out,
	char state
)
{
	switch(out)
	{
		case OUTPUT_LED:
		out ? (PORTD |= (1 << PORTD6)) : (PORTD &= ~(1 << PORTD6));
		break;
		
		case OUTPUT_1:
		out ? (PORTB |= (1 << PORTB0)) : (PORTB &= ~(1 << PORTB0));
		break;
		
		case OUTPUT_2:
		out ? (PORTD |= (1 << PORTD3)) : (PORTD &= ~(1 << PORTD3));
		break;
	}
}

/******************************************************************************/
char outputs_get(
	outputs_e out
)
{
	char ret = 0;
	switch(out)
	{
		case OUTPUT_LED:
		ret = ((PORTD >> PORTD6) & 0x01);
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