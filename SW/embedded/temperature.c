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
static unsigned char temperature_wait(
	void
)
{
	while(!(TWCR & (1 << TWINT)))
	{		
	}
	
	return TW_STATUS;
}


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
	
	/* Disable TWI interrupts */
	TWCR &= ~((1 << TWIE)); 
	
	/* Enable TWI */ 
	TWCR |= ((1 << TWEN));
	
}

/******************************************************************************/
int temperature_get(
	void
)
{
	int temp = 0;
	unsigned char status;
	unsigned char tmp;
	
	/* Wait for stop condition to be executed */
	while ((TWCR & (1 << TWSTO)))
	{
	}
	
	/* Transmit start */	
	TWCR |= ((1 << TWINT) | (1 << TWSTA));
	status = temperature_wait();
	if(TW_START != status && TW_REP_START != status)
	{
		return TEMPERATURE_ERR;
	}
	
	/* Write address */
	TWDR = TEMPERATURE_LM75_ADDR | TW_READ;	
	tmp = TWCR;	
	tmp |= ((1 << TWINT));
	tmp &= ~((1 << TWSTA));
	TWCR = tmp;	
	status = temperature_wait();
	if(TW_MR_SLA_ACK != status)
	{
		return TEMPERATURE_ERR;
	}
	
	TWCR |= ((1 << TWINT) | (1 << TWEA));
	temperature_wait();
	temp = TWDR;
	
	tmp = TWCR;
	tmp |= (1 << TWINT);
	tmp &= ~((1 << TWEA));
	TWCR = tmp;
	temp <<= 8;
	temp >>= 6;
	temperature_wait();
	tmp = TWDR;
		
	
	temp |= (tmp >> 6);
	
	
	/* Transmit stop */
	TWCR |= (1 << TWSTO);
		
	return temp;
}

