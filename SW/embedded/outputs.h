/**
 * \file    outputs.h
 * \author  Stefan Misik
 * \brief   Driver for the outputs
 *     
 */

#ifndef OUTPUTS_H_
#define OUTPUTS_H_

/**
 * \brief Outputs selector
 */
typedef enum
{
	OUTPUT_LED = 0,	/** < LED, value range : 0 - 255 */
	OUTPUT_BL,		/** < LCD backlight, value range : 0 - 255 */
	OUTPUT_1,		/** < Output No. 1, value range : 0 or 1 */
	OUTPUT_2		/** < Output No. 2, value range : 0 or 1 */
} outputs_e;

/**
 * \brief Driver initializer 
 */
void outputs_init(
	void
);

/**
 * \brief Set a new value to selected output
 *
 * \param[in] out	Selected output
 * \param[in] value	New value of a output
 */	
void outputs_set(
	outputs_e out,
	unsigned char value
);

/**
 * \brief Get the current value from selected output
 *
 * \param[in] out Selected output
 *
 * \return Current value of the selected output
 */
unsigned char outputs_get(
	outputs_e out
);


#endif /* OUTPUTS_H_ */