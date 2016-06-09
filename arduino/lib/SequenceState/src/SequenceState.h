
#include "States.h"
class SequenceState
{
public:
	SequenceState(void);
	void advance(void);
	bool isStartTimer(void);
	bool timerComplete;
	unsigned char buttonState;
	unsigned char state;
	unsigned char colour;

protected:
	unsigned char previousButtonState;
	bool isTimerComplete(void);
	void getNextColour(void);
	bool startTimer;
	unsigned char nextState;
};
