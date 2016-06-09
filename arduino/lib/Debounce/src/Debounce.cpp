// #include "stdafx.h"
#include "Debounce.h"

#define DEBOUNCE_COUNT 10

Debounce::Debounce(unsigned char defaultState)
{
	debouncedValue = defaultState;
	counter = 0;
};

unsigned char Debounce::debounce(unsigned char reading)
{
	if (counter >= DEBOUNCE_COUNT) {
		counter = 0;
		debouncedValue = reading;
	}
	else {
		if (reading == debouncedValue && counter > 0) {
			(counter)--;
		}
		if (reading != debouncedValue) {
			(counter)++;
		}
	}
	return debouncedValue;
}
