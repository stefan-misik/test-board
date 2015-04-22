/**
 * \file    alarm.c
 * \author  Stefan Misik
 * \brief   Alarms implementation
 *     
 */

#include <stddef.h>
#include "alarm.h"


/**
 * \brief Pointer to first alarm structure
 */
static volatile alarm_t * alarm_first = NULL;


/******************************************************************************/
void alarm_execute(
    alarm_time_t now
)
{
    static alarm_callback_t callback;
    static volatile alarm_t * alarm, * parent;
    
    parent = (volatile alarm_t *)&alarm_first;
    alarm = alarm_first;
    
    while (NULL != alarm)
    {
        /* If alarm should be alarmed */
        if(now == alarm->alarm_time)
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