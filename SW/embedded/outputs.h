/*
 * outputs.h
 *
 * Created: 1. 4. 2015 18:24:08
 *  Author: Stefan
 */ 


#ifndef OUTPUTS_H_
#define OUTPUTS_H_


typedef enum
{
	OUTPUT_LED = 0,
	OUTPUT_1,
	OUTPUT_2		
} outputs_e;

void outputs_init(
	void
);

void outputs_set(
	outputs_e out,
	char state
);

char outputs_get(
	outputs_e out
);


#endif /* OUTPUTS_H_ */