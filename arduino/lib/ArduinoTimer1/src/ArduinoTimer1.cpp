#include "ArduinoTimer1.h"

ArduinoTimer1::ArduinoTimer1(void)
{}

void ArduinoTimer1::setup(void)
{
  TCCR1A = 0;       // reset timer 1
  TCCR1B = 0;       // clocked on rising edge of external clock
  OCR1AH = 0xFF;    // MSByte of comparator value
  OCR1AH = 0xFF;    // LSByte of comparator value
}

void ArduinoTimer1::start(void)
{
  setMode (2, Timer1::T1_RISING, Timer1::NO_PORT);
  TIFR1 |= bit (OCF1A);    // clear interrupt flag
  TIMSK1 = bit (OCIE1A);   // interrupt on Compare A Match
}

void ArduinoTimer1::setMode (const byte mode, const byte clock, const byte port)
{
if (mode < 0 || mode > 15)  // sanity check
  return;

// reset existing flags
TCCR1A = 0;
TCCR1B = 0;

TCCR1A |= (Timer1::Modes [mode] [0]) | port;
TCCR1B |= (Timer1::Modes [mode] [1]) | clock;
} 
