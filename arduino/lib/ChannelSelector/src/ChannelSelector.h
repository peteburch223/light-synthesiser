#pragma once
#include "Debounce.h"
class ChannelSelector : public Debounce
{
public:
	ChannelSelector(unsigned char defaultState, unsigned char* portBaseAddress, unsigned char pinMask);
	unsigned char value(void);
	unsigned char test;
	unsigned char valueShiftedToRoot(void);

private:
	//unsigned char read_channel(void);
	unsigned char* PINx;
	unsigned char* DDRx;
	unsigned char* PORTx;
	unsigned char pinMask;
	unsigned char shiftsToRoot;

};