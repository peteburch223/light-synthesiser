
#include "States.h"

#ifndef _SequenceState_h
  #define _SequenceState_h

  class SequenceState
  {
  public:
  	SequenceState(void);
  	void advance(void);
    void clearStartTimer(void);
    bool isStartTimer(void);
    unsigned char getState(void);
    unsigned char getColour(void);
    bool startTimer;
  	bool timerComplete;
  	unsigned char buttonState;
  	unsigned char state;
  	unsigned char colour;

  private:
  	unsigned char previousButtonState;
  	bool isTimerComplete(void);
  	void getNextColour(void);
  	unsigned char nextState;
  };

#endif
