#pragma once

class Debounce {
public:
	Debounce(unsigned char defaultState);
	unsigned char debounce(unsigned char reading);

protected:
	int counter;
	unsigned char debouncedValue;
};
