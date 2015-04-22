/**
 * \file    alarm.h
 * \author  Stefan Misik
 * \brief   Alarms implementation
 *     
 */

#ifndef ALARM_H_
#define ALARM_H_


/**
 * \brief Time type for alarms subsystem
 */
typedef unsigned int alarm_time_t;

/**
 * \brief Alarm callback function
 */
typedef void (*alarm_callback_t)(void);

/**
 * \brief Structure holding information about alarms
 */
typedef struct timer_alarm
{
    struct timer_alarm * next_alarm;     /** < Nex alarm in linked list */
    alarm_callback_t     alarm_callback; /** < Callback function pointer */
    alarm_time_t         alarm_time;     /** < Alarm time */    
} alarm_t;


/**
 * \brief Check alarm queue for activated alarms
 * 
 * \param[in] now   Current time value
 *
 * \attention This function should be called in timer interrupt routine
 */
void alarm_execute(
    alarm_time_t now
);





#endif /* ALARM_H_ */