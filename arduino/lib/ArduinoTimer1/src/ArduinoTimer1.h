#include "Arduino.h"
#include "TimerHelpers.h"

#ifndef _ArduinoTimer1_h
  #define _ArduinoTimer1_h

  class ArduinoTimer1
  {
    public:
      ArduinoTimer1(void);
      void setup(void);
      void start(void);
      unsigned long comparator;

    private:
      void setMode (const byte mode, const byte clock, const byte port);
      void debugSerial(void);
  };
#endif
