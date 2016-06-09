#pragma once
#include "Debounce.h"

class ReadPortValue : public Debounce
{
public:
	ReadPortValue(unsigned char defaultState, unsigned char* portBaseAddress, unsigned char pinMask);
	unsigned char value(void);
	unsigned char valueShiftedToRoot(void);

private:
	unsigned char volatile * PINx;
	unsigned char * DDRx;
	unsigned char * PORTx;
	unsigned char pinMask;
	unsigned char shiftsToRoot;

};
