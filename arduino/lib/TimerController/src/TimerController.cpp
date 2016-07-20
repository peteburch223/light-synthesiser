#include "TimerController.h"
#include "arduino.h"
#include "States.h"

// configure these values for a delay of 10ms
#define WAIT_T1_COMPARATOR 0x9C3F
#define WAIT_T2_COMPARATOR 0x01
#define WAIT_T2_PRESCALER_POINTER 0x01

TimerController::TimerController(ArduinoTimer1 *timer1, ArduinoTimer2 *timer2, SequenceState *stateMachine, Duration (&arr)[16][3])
  : _timer1(timer1), _timer2(timer2), _stateMachine(stateMachine), _durations(arr)
{
}

void TimerController::checkForStart(unsigned char channel)
{
  Duration duration;

  if(_stateMachine->isStartTimer())
  {
    _stateMachine->clearStartTimer();
    switch (_stateMachine->getState())
    {

      case TEST:
        colour = 0;
        if (DEBUG) Serial.println("TimerController - Starting Timer - State: TEST");
        duration.duration = 0;
        duration.t2_prescaler_pointer = (unsigned long) WAIT_T2_PRESCALER_POINTER;
        duration.t2_comparator = (unsigned long) WAIT_T2_COMPARATOR;
        duration.t1_comparator = (unsigned long) WAIT_T1_COMPARATOR;
        setupTimers(duration);
      break;


      case WAIT:
        colour = 0;
        if (DEBUG) Serial.println("TimerController - Starting Timer - State: WAIT");
        duration.duration = 0;
        duration.t2_prescaler_pointer = (unsigned long) WAIT_T2_PRESCALER_POINTER;
        duration.t2_comparator = (unsigned long) WAIT_T2_COMPARATOR;
        duration.t1_comparator = (unsigned long) WAIT_T1_COMPARATOR;
        setupTimers(duration);
      break;

      case DISPLAY_COLOUR:
        colour = _stateMachine->getColour() + 2;
        if (DEBUG) Serial.print("TimerController:  Starting Timer: DISPLAY_COLOUR : Channel ");
        if (DEBUG) Serial.println(channel, HEX);


    	  if (DEBUG) Serial.print("Colour: ");
        if (DEBUG) Serial.println(_stateMachine->getColour(), HEX);
    		if (DEBUG) Serial.print(_durations[channel][_stateMachine->getColour()].t1_comparator, HEX);
    		if (DEBUG) Serial.print(",");
    		if (DEBUG) Serial.print(_durations[channel][_stateMachine->getColour()].t2_comparator, HEX);
    		if (DEBUG) Serial.print(",");
    		if (DEBUG) Serial.println(_durations[channel][_stateMachine->getColour()].t2_prescaler_pointer, HEX);

        duration = _durations[channel][_stateMachine->getColour()];
        setupTimers(duration);
      break;

      default:
        if (DEBUG) Serial.println("TimerController - State: DEFAULT");
    }
  }
}

void TimerController::setupTimers(Duration duration)
{
  _timer1->comparator = duration.t1_comparator;
  _timer2->prescaler = duration.t2_prescaler_pointer;
  _timer2->comparator = duration.t2_comparator;
  _timer2->setup();
  _timer1->setup();
  _timer1->start();
  _timer2->start(colour);
}
