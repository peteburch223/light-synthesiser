#include "ArduinoTimer2.h"

ArduinoTimer2::ArduinoTimer2(void)
{}

void ArduinoTimer2::setup(void)
{
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 0xFF;
  TCNT2 = 0;
}

void ArduinoTimer2::start(void)
{
  setMode (4, prescaler, Timer2::TOGGLE_A_ON_COMPARE);
}

void ArduinoTimer2::setMode (const byte mode, const byte clock, const byte port)
{
if (mode < 0 || mode > 7)  // sanity check
  return;

// reset existing flags
TCCR2A = 0;
TCCR2B = 0;

TCCR2A |= (Timer2::Modes [mode] [0]) | port;
TCCR2B |= (Timer2::Modes [mode] [1]) | clock;
}
