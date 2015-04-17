/**
 * \file    temperature.c
 * \author  Stefan Misik
 * \brief   Driver for the IIC temperature sensor
 *     
 */


#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#include "temperature.h"
#include "outputs.h"

/**
 * \brief Measurement state variable type
 */
typedef enum
{
	TEMPERATURE_IDLE = 0,	/** < Temperature measurement idle */
	TEMPERATURE_START,		/** < Start condition transmitted */
	TEMPERATURE_ADDRESS,	/** < Address transmitted */
	TEMPERATURE_READ_1,		/** < First byte received */
	TEMPERATURE_READ_2		/** < Second byte received */
} temperature_stat_e;

/**
 * \brief Address of the LM75 chip
 */
#define TEMPERATURE_LM75_ADDR	0x9E

/**
 * \brief Current temperature value
 */
static int temperature = 0;

/**
 * \brief Temperature measurement status
 */
static temperature_stat_e temperature_stat = TEMPERATURE_IDLE;


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
	
	/* Enable TWI interrupts */
	TWCR |= ((1 << TWIE)); 
	asm volatile("SEI \n\t");
	
	/* Enable TWI */ 
	TWCR |= ((1 << TWEN));		
}

/******************************************************************************/
int temperature_get(
	void
)
{
	int ret;
	unsigned char tmp;
	
	/* Disable interrupts */
	tmp = SREG;
	asm volatile("CLI \n\t");
	
	/* Get the current temperature */
	ret = temperature;
	
	/* Potentially re-enable interrupts */
	SREG = tmp;
	
	return ret;
}

/******************************************************************************/
char temperature_update(
	void
)
{
	char ret = temperature_stat;
	
	/* Check if measurement is not in process */
	if(TEMPERATURE_IDLE == temperature_stat)
	{
		/* Wait for stop condition to be executed */
		while ((TWCR & (1 << TWSTO)))
		{
		}

		/* Change measurement status */
		temperature_stat = TEMPERATURE_START;
		
		/* Transmit start */
		TWCR |= ((1 << TWINT) | (1 << TWSTA));		
	}
	else
	{
		ret = 1;
	}
	
	return ret;
}

/******************************************************************************/
ISR(TWI_vect)
{
	unsigned char status;
	static int tmp_temp;
	unsigned char tmp;
	
	/* Get status */
	status = TW_STATUS;	
	
	switch(temperature_stat)
	{
		case TEMPERATURE_START:
			if(TW_START != status && TW_REP_START != status)
			{
				temperature_stat = TEMPERATURE_IDLE;
			}
			else
			{				
				/* Change status */
				temperature_stat = TEMPERATURE_ADDRESS;
				/* Write address */
				TWDR = TEMPERATURE_LM75_ADDR | TW_READ;
				tmp = TWCR;
				tmp |= ((1 << TWINT));
				tmp &= ~((1 << TWSTA));
				TWCR = tmp;
			}			
		break;
		
		case TEMPERATURE_ADDRESS:
			if(TW_MR_SLA_ACK != status)
			{				
				temperature_stat = TEMPERATURE_IDLE;
			}
			else
			{
				/* Change status */
				temperature_stat = TEMPERATURE_READ_1;
				
				/* Start receiving */
				TWCR |= ((1 << TWINT) | (1 << TWEA));
			}
		break;
		
		case TEMPERATURE_READ_1:
			if(TW_MR_DATA_ACK != status)
			{
				temperature_stat = TEMPERATURE_IDLE;
			}
			else
			{
				/* Change status */
				temperature_stat = TEMPERATURE_READ_2;
				
				tmp_temp = (int)TWDR;
				tmp = TWCR;
				tmp |= (1 << TWINT);
				tmp &= ~((1 << TWEA));
				TWCR = tmp;
			}
		break;
		
		case TEMPERATURE_READ_2:
			temperature_stat = TEMPERATURE_IDLE;
			
			if(TW_MR_DATA_NACK == status)			
			{
				tmp_temp <<= 8;
				tmp_temp >>= 6;
				tmp = TWDR;				
				tmp_temp |= (int)(tmp >> 6);
				
				/* Write new reading */
				temperature = tmp_temp;				
			}
			
			/* Transmit stop */
			TWCR |= (1 << TWSTO);
		break;
	}	
}