// #include "stdafx.h"
#include "arduino.h"
#include "SequenceState.h"

// #ifndef LOW
// #define LOW 0
// #endif // !LOW
//
// #ifndef HIGH
// #define HIGH 1
// #endif // !LOW

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
		if (buttonState != previousButtonState && buttonState == HIGH)
		{
			Serial.println("...triggered");
			startTimer = true;
			state = WAIT;
		}
		break;

	case WAIT:
		if (isTimerComplete())
		{
			Serial.println("...WAIT timer complete");
			getNextColour();
			startTimer = true;
			state = DISPLAY_COLOUR;
		}
		break;

	case DISPLAY_COLOUR:
		// Serial.println("State: DISPLAY_COLOUR");

		if (isTimerComplete())
    {
			Serial.println("...DISPLAY_COLOUR timer complete");
			if (colour == BLUE)
				state = AWAIT_COMMAND;
			else
				state = WAIT;
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
bool SequenceState::isTimerComplete(void)
{
	bool temp = timerComplete;
	// Serial.print("Timer Complete: ");
	// Serial.println(temp, HEX);
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
