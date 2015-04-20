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
 * \return	Temperature value in dg. C * 4, 
			TEMPERATURE_ERR on communication error
 */
int temperature_get(
	void
);

/**
 * \brief Starts the update process of temperature reading
 *
 * \return 0 on success
 */
char temperature_update(
	void
);

/**
 * \brief Get the status of temperature measurement
 *
 * \return 0 when not busy
 */
char temperature_busy(
	void
);

#endif /* TEMPERATURE_H_ */