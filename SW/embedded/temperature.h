/**
 * \file    temperature.h
 * \author  Stefan Misik
 * \brief   Driver for the IIC temperature sensor
 *     
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

/**
 * \brief Initialization of temperature module
 */
void temperature_init(
	void
);

/**
 * \brief Read current temperature value
 *
 * \return Temperature value in dg. C * 2
 */
int temperature_get(
	void
);



#endif /* TEMPERATURE_H_ */