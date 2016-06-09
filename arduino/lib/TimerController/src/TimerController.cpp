#include "TimerController.h"

// configure these values for a delay of 10ms
#define WAIT_T1_COMPARATOR 0x0
#define WAIT_T2_PRESCALER_POINTER 0x0
#define WAIT_T2_COMPARATOR 0x0


TimerController::TimerController(ArduinoTimer1 timer1, ArduinoTimer1 timer2, Duration durations[][3])
{
  _timer1 = timer1;
  _timer2 = timer2;
  _durations = durations;
}

void TimerController::checkForStart(SequenceState stateMachine, unsigned char channel)
{
  Duration duration;
  if(stateMachine.isStartTimer())
  {
    switch (stateMachine.state)
    {
      case WAIT:
        duration = {0,
                   (unsigned long) WAIT_T2_PRESCALER_POINTER,
                   (unsigned long) WAIT_T2_COMPARATOR,
                   (unsigned long) WAIT_T1_COMPARATOR};

      break;

      case COLOUR:
        duration = _durations[channel][stateMachine.colour]
      break;
    }
    _timer1.setup();
    _timer2.setup();
    _timer1.comparator = duration.t1_comparator;
    _timer2.prescaler = duration.t2_prescaler_pointer;
    _timer2.comparator = duration.t2_comparator;
    _timer1.start();
    _timer2.start();
  }
}
