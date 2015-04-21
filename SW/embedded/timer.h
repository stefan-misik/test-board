/**
 * \file    timer.h
 * \author  Stefan Misik
 * \brief   Timing tool
 *     
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

/**
 * \brief Type for storing long time format
 */
typedef uint64_t long_time_t;

/**
 * \brief Type for storing short time format
 */
typedef uint32_t short_time_t;

/**
 * \brief Structure holding information about alarms
 */
typedef struct timer_alarm
{
    struct timer_alarm *    next_alarm;    /** < Nex alarm in linked list */
    void                    (*alarm_callback)(void); /** < Callback 
                                                      * function pointer */
    long_time_t             alarm_time; /** < Alarm time */    
} timer_alarm_t;


/**
 * \brief Initialize timer module 
 */
void timer_init(
    void
);

/**
 * \brief Gets the current time in long format
 *
 * \param[out] t    Pointer to variable that obtains current time value 
 *                    in milliseconds 
 */
void timer_get_long(
    long_time_t * t
);

/**
 * \brief Gets the current time in long format
 *
 * \return Short format of current time in seconds
 */
short_time_t timer_get_short(
    void
);

/**
 * \brief Sets the current time in long format
 *
 * \param[int] t Pointer to the new current time value
 */
void timer_set_long(
    const long_time_t * t
);


#endif /* TIMER_H_ */