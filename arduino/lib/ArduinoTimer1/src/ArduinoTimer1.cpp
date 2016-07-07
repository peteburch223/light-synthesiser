#include "ArduinoTimer1.h"
#include "arduino.h"

#define T1 5

ArduinoTimer1::ArduinoTimer1(void)
{
      pinMode(T1, INPUT);
}

void ArduinoTimer1::setup(void)
{
  Serial.println("...configuring Timer 1");
  Serial.print("......Comparator: ");
  Serial.println(comparator, HEX);

  byte LSByte = comparator & 0xFF;
  byte MSByte = (comparator >> 8) & 0xFF;

  Serial.print("......LSByte: ");
  Serial.println(LSByte, HEX);

  Serial.print("......MSByte: ");
  Serial.println(MSByte, HEX);
  TCCR1A = 0;       // reset timer 1
  TCCR1B = 0;       // clocked on rising edge of external clock
  OCR1AH = LSByte;    // MSByte of comparator value
  OCR1AH = MSByte;    // LSByte of comparator value
}

void ArduinoTimer1::start(void)
{
  Serial.println("...starting Timer 1");
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
Serial.println("...Timer 1 mode set");
}
