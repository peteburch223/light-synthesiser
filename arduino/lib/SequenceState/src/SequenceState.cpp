// #include "stdafx.h"
#include "arduino.h"
#include "SequenceState.h"

SequenceState::SequenceState()
{
	buttonState = LOW;
	previousButtonState = LOW;
	state = AWAIT_COMMAND;
	timerComplete = false;
	startTimer = false;
	colour = NO_COLOUR;
	DDRD |= 0x1C;				// set digital IO 2-4 as outputs for LEDs
	PIND &= 0xE3;
}

void SequenceState::advance(void)
{
	switch (state)
	{
	case AWAIT_COMMAND:
		// Serial.println("State: AWAIT_COMMAND");
		if (buttonState != previousButtonState && buttonState == HIGH)
		{
			Serial.println("...triggered");
			startTimer = true;
			state = WAIT;
		}
		break;

	case WAIT:
		// Serial.println("State: WAIT");
		if (isTimerComplete())
		{
			clearTimerComplete();
			Serial.println("...WAIT timer complete");
			getNextColour();
			startTimer = true;
			state = DISPLAY_COLOUR;
		}
		break;

	case DISPLAY_COLOUR:
		//  Serial.println("State: DISPLAY_COLOUR");

		if (isTimerComplete())
    {
			clearTimerComplete();
			Serial.println("...DISPLAY_COLOUR timer complete");
			if (colour == BLUE)
			{
				Serial.println(" **** colours completed ****");
				getNextColour();
				state = AWAIT_COMMAND;
			}
			else
			{
				Serial.println("...waiting for next colour");
				startTimer = true;
				state = WAIT;
			}
    }
		break;
	}
	previousButtonState = buttonState;
}

void SequenceState::clearStartTimer(void)
{
	startTimer = false;
}

unsigned char SequenceState::getState(void)
{
	return state;
}

unsigned char SequenceState::getColour(void)
{
	return colour;
}
bool SequenceState::isStartTimer(void)
{
	return startTimer;
}

void SequenceState::setTimerComplete(void)
{
	timerComplete = true;
}

bool SequenceState::isTimerComplete(void)
{
	return timerComplete;
}

void SequenceState::clearTimerComplete(void)
{
	timerComplete = false;
}

void SequenceState::getNextColour(void)
{
	switch (colour)
	{
	case NO_COLOUR:
		colour = RED;
		break;
	case RED:
		colour = GREEN;
		break;
	case GREEN:
		colour = BLUE;
		break;
	case BLUE:
		colour = NO_COLOUR;
		break;
	}
}
