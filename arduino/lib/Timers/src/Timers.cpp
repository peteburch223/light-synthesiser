#include "Timers.h"

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
  Timer1::setMode (2, Timer1::T1_RISING, Timer1::NO_PORT);
  TIFR1 |= bit (OCF1A);    // clear interrupt flag
  TIMSK1 = bit (OCIE1A);   // interrupt on Compare A Match
}

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
  Timer2::setMode (4, prescaler, Timer2::TOGGLE_A_ON_COMPARE);
}
