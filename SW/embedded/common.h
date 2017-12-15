/**
 * \file    common.h
 * \author  Stefan Misik
 * \brief   Common definitions
 *     
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 20000000UL

/* Make sure a MCU is selected, mostly due to code analyzer in NetBeans */
#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>




#endif /* COMMON_H_ */