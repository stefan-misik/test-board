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


/**
 * \brief Address of the LM75 chip
 */
#define TEMPERATURE_LM75_ADDR    0x9E

/**
 * \brief Current temperature value
 */
static volatile int temperature = 0;

/**
 * \brief Temperature measurement status
 */
static enum
{
    TEMPERATURE_IDLE = 0,    /** < Temperature measurement idle */
    TEMPERATURE_START,        /** < Start condition transmitted */
    TEMPERATURE_ADDRESS,    /** < Address transmitted */
    TEMPERATURE_READ_1,        /** < First byte received */
    TEMPERATURE_READ_2        /** < Second byte received */
} temperature_stat = TEMPERATURE_IDLE;


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
    char busy = temperature_busy();
    
    /* Check if measurement is not in process */
    if(0 == busy)
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
    
    return busy;
}

/******************************************************************************/
char temperature_busy(
    void
)
{
    return TEMPERATURE_IDLE == temperature_stat ? 0 : 1;
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
        /* Start condition should have been generated */
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
        
        /* Slave address + R should have been sent */
        case TEMPERATURE_ADDRESS:
            if(TW_MR_SLA_ACK != status)
            {                
                temperature_stat = TEMPERATURE_IDLE;
                
                /* Transmit stop */
                TWCR |= (1 << TWSTO);
            }
            else
            {
                /* Change status */
                temperature_stat = TEMPERATURE_READ_1;
                
                /* Start receiving */
                TWCR |= ((1 << TWINT) | (1 << TWEA));
            }
        break;
        
        /* First byte should have been received */
        case TEMPERATURE_READ_1:
            if(TW_MR_DATA_ACK != status)
            {
                temperature_stat = TEMPERATURE_IDLE;
                
                /* Transmit stop */
                TWCR |= (1 << TWSTO);
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
        
        /* Second byte should have been received */
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
        
        /* Default operation = unknown state */
        default:
            temperature_stat = TEMPERATURE_IDLE;
            
            /* Transmit stop */
            TWCR |= (1 << TWSTO);
        break;
    }    
}
