// #include "stdafx.h"
#include "arduino.h"
#include "SequenceState.h"

#ifndef LOW
#define LOW 0
#endif // !LOW

#ifndef HIGH
#define HIGH 1
#endif // !LOW

SequenceState::SequenceState()
{
	buttonState = LOW;
	previousButtonState = LOW;
	state = AWAIT_COMMAND;
	timerComplete = false;
	startTimer = false;
	colour = NO_COLOUR;
}

void SequenceState::advance(void)
{
	switch (state)
	{
	case AWAIT_COMMAND:
    Serial.println("State: AWAIT_COMMAND");
		if (buttonState != previousButtonState && buttonState == HIGH)
		{
			Serial.println("...triggered");
			startTimer = true;
			state = WAIT;
		}
		break;

	case WAIT:
		Serial.println("State: WAIT");

		if (isTimerComplete())
		{
			Serial.println("...timer complete");
			getNextColour();
			startTimer = true;
			state = DISPLAY_COLOUR;
		}
		break;

	case DISPLAY_COLOUR:
		Serial.println("State: DISPLAY_COLOUR");

		if (isTimerComplete())
    {
			Serial.println("...timer complete");
			if (colour == BLUE)
				state = AWAIT_COMMAND;
			else
				state = WAIT;
    }
		break;

	}
	previousButtonState = buttonState;
}

bool SequenceState::isStartTimer(void)
{
	bool temp = startTimer;
	startTimer = false;
	return temp;

}

bool SequenceState::isTimerComplete(void)
{
	bool temp = timerComplete;
	timerComplete = false;
	return temp;
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
