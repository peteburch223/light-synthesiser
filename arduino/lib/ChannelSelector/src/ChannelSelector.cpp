// #include "stdafx.h"
#include "ChannelSelector.h"

ChannelSelector::ChannelSelector(unsigned char defaultState, unsigned char* portBaseAddress, unsigned char pinMaskValue )
	: Debounce(defaultState)
{
	pinMask = pinMaskValue;
	PINx = portBaseAddress;
	DDRx = portBaseAddress + 1;
	PORTx = portBaseAddress + 2;

	*DDRx &= ~pinMask;
	*PORTx &= ~pinMask;

	shiftsToRoot = 0;
	while ((pinMaskValue & 0x1) == 0)
	{
		pinMaskValue = pinMaskValue >> 1;
		shiftsToRoot += 1;
	}
}

unsigned char ChannelSelector::value(void)
{
	return *PINx & pinMask;
}

unsigned char ChannelSelector::valueShiftedToRoot(void)
{
	return value() >> shiftsToRoot;
}
