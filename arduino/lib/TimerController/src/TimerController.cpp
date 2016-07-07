#include "TimerController.h"
#include "arduino.h"

// configure these values for a delay of 10ms
#define WAIT_T1_COMPARATOR 0x0
#define WAIT_T2_PRESCALER_POINTER 0x06
#define WAIT_T2_COMPARATOR 0xFE

TimerController::TimerController(ArduinoTimer1 *timer1, ArduinoTimer2 *timer2, SequenceState *stateMachine, Duration (&arr)[16][3])
  : _timer1(timer1), _timer2(timer2), _stateMachine(stateMachine), _durations(arr)
{
}

void TimerController::checkForStart(unsigned char channel)
{
  Duration duration;

  if(_stateMachine->isStartTimer())
  {
    Serial.println("TimerController - Starting Timer");
    _stateMachine->clearStartTimer();
    switch (_stateMachine->getState())
    {
      case WAIT:
        Serial.println("TimerController State: WAIT");
        duration.duration = 0;
        duration.t2_prescaler_pointer = (unsigned long) WAIT_T2_PRESCALER_POINTER;
        duration.t2_comparator = (unsigned long) WAIT_T2_COMPARATOR;
        duration.t1_comparator = (unsigned long) WAIT_T1_COMPARATOR;
        setupTimers(duration);
      break;

      case DISPLAY_COLOUR:
        Serial.println("TimerController State: DISPLAY_COLOUR");
        duration = _durations[channel][_stateMachine->getColour()];
        setupTimers(duration);
      break;

      default:
        Serial.println("TimerController State: DEFAULT");
    }
  }
  // Serial.println("end of checkForStart");
}

void TimerController::setupTimers(Duration duration)
{
  Serial.println("Set up and start timers");
  _timer1->comparator = duration.t1_comparator;
  _timer2->prescaler = duration.t2_prescaler_pointer;
  _timer2->comparator = duration.t2_comparator;
  _timer1->setup();
  _timer2->setup();
  _timer1->start();
  _timer2->start();
}
