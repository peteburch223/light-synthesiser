// #include "stdafx.h"
#include "arduino.h"
#include "ReadPortValue.h"

ReadPortValue::ReadPortValue(unsigned char defaultState, unsigned char * portBaseAddress, unsigned char pinMaskValue )
	: Debounce(~defaultState)
{
	pinMask = pinMaskValue;
	PINx = (unsigned char volatile *)portBaseAddress;
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

unsigned char ReadPortValue::value(void)
{
	return debounce(*PINx & pinMask);
}

unsigned char ReadPortValue::valueShiftedToRoot(void)
{
	// Serial.println(value(), HEX);
	return ~(value() >> shiftsToRoot) & (pinMask >> shiftsToRoot);
}
