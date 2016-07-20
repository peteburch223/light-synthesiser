#include "ArduinoTimer2.h"
#include "arduino.h"
#include "States.h"

#define OC2A 11
#define TIMER2_MODE 2     // CTC, top = OCR2A

ArduinoTimer2::ArduinoTimer2(void)
{
  digitalWrite(OC2A, LOW);
  pinMode(OC2A, OUTPUT);
  GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); // halt all timers
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  OCR2A = 0x00;
  TCNT2 = 0x00;
}

void ArduinoTimer2::setup(void)
{
  GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC);
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = comparator;
  TCNT2 = 0;

}

void ArduinoTimer2::start(unsigned char colour)
{
  setMode (TIMER2_MODE, prescaler, Timer2::TOGGLE_A_ON_COMPARE);
  PORTD |= (1 << colour);
  GTCCR = 0x00;
  if (DEBUG) debugSerial();
}

void ArduinoTimer2::setMode (const byte mode, const byte clock, const byte port)
{
if (mode < 0 || mode > 7) return;

TCCR2A |= (Timer2::Modes [mode] [0]) | port;
TCCR2B |= (Timer2::Modes [mode] [1]) | clock;
}

void ArduinoTimer2::debugSerial(void)
{
  Serial.print("......OCR2A: ");
  Serial.println(OCR2A, HEX);
  Serial.print("......TCCR2A: ");
  Serial.println(TCCR2A, HEX);
  Serial.print("......TCCR2B: ");
  Serial.println(TCCR2B, HEX);
}
