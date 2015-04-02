/**
 * \file    temperature.c
 * \author  Stefan Misik
 * \brief   Driver for the IIC temperature sensor
 *     
 */


#include <avr/io.h>
#include <util/twi.h>
#include "temperature.h"

/**
 * \brief Address of the LM75 chip
 */
#define TEMPERATURE_LM75_ADDR	0x9E

/******************************************************************************/



/******************************************************************************/
void temperature_init(
	void
)
{
	/* Set SCA and SDA as input */
	DDRC &= ~((1 << DDC4) | (1 << DDC5));
	
	/* Disable internal pull-ups */
	PORTC &= ~((1 << PORTC4) | (1 << PORTC5));	
	
	/* Set bitrate */
	/* TWPS1:0 = 00 (/1), TWBR7:0 = 17 => 20Mhz / (16 + 2 * 17 * 1) = 400 Khz */
	TWBR = 17;
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	
}

/******************************************************************************/
int temperature_get(
	void
)
{
	int temp = 0;
	
	
	
	return temp;
}

