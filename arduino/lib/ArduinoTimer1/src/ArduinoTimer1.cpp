#include "ArduinoTimer1.h"
#include "arduino.h"
#include "States.h"

#define T1 5
#define TIMER1_MODE 4     // CTC, top = OCR1A


ArduinoTimer1::ArduinoTimer1(void)
{
  pinMode(T1, INPUT);
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  OCR1A = 0x00;
	TCNT1 = 0x00;
  TIMSK1 = 0x00;
  TIFR1 = 0x00;
}

void ArduinoTimer1::setup(void)
{
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = comparator;
  TCNT1 = 0;
}

void ArduinoTimer1::start(void)
{
  setMode (TIMER1_MODE, Timer1::T1_RISING, Timer1::NO_PORT);
  TIFR1 |= bit (OCF1A);    // clear interrupt flag
  TIMSK1 = bit (OCIE1A);   // interrupt on Compare A Match
  if (DEBUG) debugSerial();
}

void ArduinoTimer1::setMode (const byte mode, const byte clock, const byte port)
{
if (mode < 0 || mode > 15)  return;

TCCR1A |= (Timer1::Modes [mode] [0]) | port;
TCCR1B |= (Timer1::Modes [mode] [1]) | clock;
}

void  ArduinoTimer1::debugSerial(void)
{
  Serial.print("......TCCR1A: ");
  Serial.println(TCCR1A, HEX);
  Serial.print("......TCCR1B: ");
  Serial.println(TCCR1B, HEX);
  Serial.print("......TIFR1: ");
  Serial.println(TIFR1, HEX);
  Serial.print("......TIMSK1: ");
  Serial.println(TIMSK1, HEX);
  Serial.print("......OCR1A: ");
  Serial.println(OCR1A, HEX);
}
