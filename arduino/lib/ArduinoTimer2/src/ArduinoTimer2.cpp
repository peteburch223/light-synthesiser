#include "ArduinoTimer2.h"
#include "arduino.h"
#include "States.h"

#define OC2A 11

ArduinoTimer2::ArduinoTimer2(void)
{
    pinMode(OC2A, OUTPUT);
    TCCR2A = 0x00;
    TCCR2B = 0x00;
    OCR2A = 0x00;
    TCNT2 = 0x00;
}

void ArduinoTimer2::setup(void)
{
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = comparator;
  TCNT2 = 0;
}

void ArduinoTimer2::start(unsigned char colour)
{
  setMode (2, prescaler, Timer2::TOGGLE_A_ON_COMPARE);
  PORTD |= (1 << colour);
  if (DEBUG) Serial.print("......OCR2A: ");
  if (DEBUG) Serial.println(OCR2A, HEX);
  if (DEBUG) Serial.print("......TCCR2A: ");
  if (DEBUG) Serial.println(TCCR2A, HEX);
  if (DEBUG) Serial.print("......TCCR2B: ");
  if (DEBUG) Serial.println(TCCR2B, HEX);
}

void ArduinoTimer2::setMode (const byte mode, const byte clock, const byte port)
{
if (mode < 0 || mode > 7)  // sanity check
  return;

TCCR2A = 0;
TCCR2B = 0;

TCCR2A |= (Timer2::Modes [mode] [0]) | port;
TCCR2B |= (Timer2::Modes [mode] [1]) | clock;
}
