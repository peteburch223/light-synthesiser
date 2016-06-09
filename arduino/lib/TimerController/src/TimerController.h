// #include "TimerHelpers.h"
#include "Arduino.h"
#include "SequenceState.h"
#include "Timers.h"
#include "States.h"
#include "Duration.h"

class TimerController
{
public:
  TimerController(ArduinoTimer1 timer1, ArduinoTimer2 timer2, Duration durations[][3]);
  void checkForStart(SequenceState stateMachine, unsigned char channel);

private:
  ArduinoTimer1 _timer1;
  ArduinoTimer2 _timer2;
};
