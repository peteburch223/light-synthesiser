
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
    void setTimerComplete(void);
    unsigned char buttonState;

  private:
  	unsigned char previousButtonState;
  	bool isTimerComplete(void);
    void clearTimerComplete(void);
  	void getNextColour(void);
  	unsigned char nextState;
    bool startTimer;
    bool timerComplete;

    unsigned char state;
    unsigned char colour;
  };

#endif
