/**
 * \file    timer.c
 * \author  Stefan Misik
 * \brief   Timing tool
 *     
 */

#include "common.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <stddef.h>

/**
 * \brief Timer overflow period in milliseconds
 */
#define TIMER_PERIOD 64

/**
 * \brief Rounding to whole periods
 */
#define TIMER_ROUND_TO_PERIODS(val) ((val) & (~(TIMER_PERIOD-1)))

/**
 * \brief Pointer to first alarm structure
 */
static volatile timer_alarm_t * first_alarm = NULL;

/**
 * \brief Current time in milliseconds 
 */
static volatile long_time_t timer_now;

/******************************************************************************/
void timer_init(
    void
)
{
    unsigned char tmp;
    timer_now = 0;
    
    /* Reset counter */ 
    TCNT1 = 0;
    
    /* Set the max value */
    ICR1 = (F_CPU / 1000) - 1;
    
    /* Set Interrupt mask */
    tmp = TIMSK1;    
    tmp &= ~((1 << OCIE1B) | (1 << OCIE1A) | (1 << TOIE1));
    tmp |= (1 << ICIE1);
    TIMSK1 = tmp;
    
    /* Enable interrupts */
    asm volatile("SEI \n\t");
    
    /* Disconnect OC1A/B */    
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) |
            (1 << WGM11) | (1 << WGM10));
            
    /* Clear output force flags */
    TCCR1C &= ~((1 << FOC1A) | (1 << FOC1B));
    
    /* Select CTC mode, prescaler set to CLKio/64 */
    tmp = TCCR1B;    
    tmp &= ~((1 << ICNC1) | (1 << CS12) | (1 << CS11) | (1 << CS10));
    tmp |= ((1 << WGM13) | (1 << WGM12) | (1 << CS11)| (1 << CS10));
    TCCR1B = tmp;    
}

/******************************************************************************/
void timer_get_long(
    long_time_t * t
)
{
    unsigned char tmp;
    
    /* Disable interrupts */
    tmp = SREG;
    asm volatile("CLI \n\t");
    
    /* Get the current time */
    *t = timer_now;
    
    /* Potentially re-enable interrupts */
    SREG = tmp;    
}

/******************************************************************************/
short_time_t timer_get_short(
    void
)
{
    long_time_t t;
    
    /* Obtain long time */
    timer_get_long(&t);
    
    return (short_time_t)(t / ((long_time_t)1000));    
}

/******************************************************************************/
void timer_set_long(
    const long_time_t * t
)
{
    unsigned char tmp;
    
    /* Disable interrupts */
    tmp = SREG;
    asm volatile("CLI \n\t");
    
    /* Set the current time */
    timer_now = *t;
    
    /* Potentially re-enable interrupts */
    SREG = tmp;
}


/******************************************************************************/
ISR(TIMER1_CAPT_vect)
{
    static timer_callback callback;
    static timer_alarm_t * alarm, * parent;
    
    /* Increment timer */
    timer_now += TIMER_PERIOD;
    
    
    parent = (timer_alarm_t *)&first_alarm;
    alarm = first_alarm;
    
    while (NULL != alarm)
    {
        /* If alarm should be alarmed */
        if(timer_now == alarm->alarm_time)
        {
            /* Get the callback */
            callback = alarm->alarm_callback;
            /* Remove alarm from the queue */
            parent->next_alarm = alarm->next_alarm;
            /* Call the callback */
            callback();
        }
        else
        {
            break;
        }
        
        /* Move to next alarm */
        parent = alarm;
        alarm = alarm->next_alarm;        
    }    
}
