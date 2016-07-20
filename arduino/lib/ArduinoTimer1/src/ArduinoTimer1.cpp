#include "ArduinoTimer1.h"
#include "arduino.h"
#include "States.h"

#define T1 5

ArduinoTimer1::ArduinoTimer1(void)
{
  pinMode(T1, INPUT);
  setMode (4, Timer1::T1_RISING, Timer1::NO_PORT);
  OCR1A = 0x00;
	TCNT1 = 0x00;
  TIMSK1 = 0x00;
  TIFR1 = 0x00;
}

void ArduinoTimer1::setup(void)
{
  // byte LSByte = comparator & 0xFF;
  // byte MSByte = (comparator >> 8) & 0xFF;




  if (DEBUG) Serial.print("......comparator: ");
  if (DEBUG) Serial.println(comparator, HEX);

  OCR1A = comparator;

  TCCR1A = 0;       // reset timer 1
  TCCR1B = 0;       // clocked on rising edge of external clock

  unsigned int tempOCR1A;
  tempOCR1A = OCR1A;
  if (DEBUG) Serial.print("......OCR1A: ");
  if (DEBUG) Serial.println(tempOCR1A, HEX);

}

void ArduinoTimer1::start(void)
{
  // Serial.println("...starting Timer 1");
  setMode (4, Timer1::T1_RISING, Timer1::NO_PORT);
  TIFR1 |= bit (OCF1A);    // clear interrupt flag
  TIMSK1 = bit (OCIE1A);   // interrupt on Compare A Match

  if (DEBUG) Serial.print("......TCCR1A: ");
  if (DEBUG) Serial.println(TCCR1A, HEX);
  if (DEBUG) Serial.print("......TCCR1B: ");
  if (DEBUG) Serial.println(TCCR1B, HEX);
  if (DEBUG) Serial.print("......TIFR1: ");
  if (DEBUG) Serial.println(TIFR1, HEX);
  if (DEBUG) Serial.print("......TIMSK1: ");
  if (DEBUG) Serial.println(TIMSK1, HEX);
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
