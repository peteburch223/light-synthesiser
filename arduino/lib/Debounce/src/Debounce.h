#pragma once

class Debounce {
public:
	Debounce(unsigned char defaultState);
	void debounced_state(unsigned char reading);
	unsigned char current_state;

protected:
	int counter;
};
