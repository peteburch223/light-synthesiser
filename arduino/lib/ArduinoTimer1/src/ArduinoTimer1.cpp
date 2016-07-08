#include "ArduinoTimer1.h"
#include "arduino.h"

#define T1 5

ArduinoTimer1::ArduinoTimer1(void)
{
  pinMode(T1, INPUT);
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  OCR1A = 0x00;
	TCNT1 = 0x00;
}

void ArduinoTimer1::setup(void)
{
  // byte LSByte = comparator & 0xFF;
  // byte MSByte = (comparator >> 8) & 0xFF;


  unsigned int tempOCR1A;



  Serial.print("......comparator: ");
  Serial.println(comparator, HEX);
  // Serial.print("......LSByte: ");
  // Serial.println(LSByte, HEX);

  OCR1A = comparator;

  TCCR1A = 0;       // reset timer 1
  TCCR1B = 0;       // clocked on rising edge of external clock
  // OCR1AL = LSByte;    // MSByte of comparator value
  // OCR1AH = MSByte;    // LSByte of comparator value

  tempOCR1A = OCR1A;
  Serial.print("......OCR1A: ");
  Serial.println(tempOCR1A, HEX);

}

void ArduinoTimer1::start(void)
{
  // Serial.println("...starting Timer 1");
  setMode (4, Timer1::T1_RISING, Timer1::NO_PORT);
  TIFR1 |= bit (OCF1A);    // clear interrupt flag
  TIMSK1 = bit (OCIE1A);   // interrupt on Compare A Match

  Serial.print("......TCCR1A: ");
  Serial.println(TCCR1A, HEX);
  Serial.print("......TCCR1B: ");
  Serial.println(TCCR1B, HEX);
  Serial.print("......TIFR1: ");
  Serial.println(TIFR1, HEX);
  Serial.print("......TIMSK1: ");
  Serial.println(TIMSK1, HEX);
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
