// #include "stdafx.h"
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
			startTimer = true;
			state = WAIT;
		}
		break;

	case WAIT:
		if (isTimerComplete())
		{
			getNextColour();
			startTimer = true;
			state = DISPLAY_COLOUR;
		}
		break;

	case DISPLAY_COLOUR:
		if (isTimerComplete())
    {
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
