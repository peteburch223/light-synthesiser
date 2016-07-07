// #include "TimerHelpers.h"
#include "Arduino.h"
#include "SequenceState.h"
#include "ArduinoTimer1.h"
#include "ArduinoTimer2.h"
#include "States.h"
#include "Duration.h"

#ifndef _TimerController_h
  #define _TimerController_h

  class TimerController
  {
  public:
    TimerController(ArduinoTimer1 *timer1, ArduinoTimer2 *timer2, SequenceState *stateMachine, Duration (&arr)[16][3]);
    void checkForStart(unsigned char channel);

  private:
    void setupTimers(Duration duration);
    unsigned char colour;
    ArduinoTimer1 * _timer1;
    ArduinoTimer2 * _timer2;
    SequenceState * _stateMachine;
    Duration (&_durations)[16][3];
  };

#endif
