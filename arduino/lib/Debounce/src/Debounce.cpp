// #include "stdafx.h"
#include "Debounce.h"

#define DEBOUNCE_COUNT 10

Debounce::Debounce(unsigned char defaultState)
{
	current_state = defaultState;
	counter = 0;
};

void Debounce::debounced_state(unsigned char reading)
{
	if (counter >= DEBOUNCE_COUNT) {
		counter = 0;
		current_state = reading;
	}
	else {
		if (reading == current_state && counter > 0) {
			(counter)--;
		}
		if (reading != current_state) {
			(counter)++;
		}
	}
}
