/**
 * \file    temperature.h
 * \author  Stefan Misik
 * \brief   Driver for the IIC temperature sensor
 *     
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

/**
 * \brief Error code
 */
#define TEMPERATURE_ERR	0x8000

/**
 * \brief Initialization of temperature module
 */
void temperature_init(
	void
);

/**
 * \brief Read current temperature value
 *
 * \return	Temperature value in dg. C * 2, 
			TEMPERATURE_ERR on communication error
 */
int temperature_get(
	void
);



#endif /* TEMPERATURE_H_ */