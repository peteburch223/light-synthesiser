#include "ArduinoTimer2.h"
#include "arduino.h"

#define OC2A 11

ArduinoTimer2::ArduinoTimer2(void)
{
    pinMode(OC2A, OUTPUT);
}

void ArduinoTimer2::setup(void)
{
  Serial.println("...configuring Timer 2");
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = comparator;
  TCNT2 = 0;
  Serial.print("......Comparator: ");
  Serial.println(comparator, HEX);
}

void ArduinoTimer2::start(void)
{
  Serial.println("...starting Timer 2");
  Serial.print("......Prescaler: ");
  Serial.println(prescaler, HEX);
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
Serial.println("...Timer 2 mode set");
}
