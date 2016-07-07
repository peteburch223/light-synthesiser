#include "Arduino.h"
#include "TimerHelpers.h"

#ifndef _ArduinoTimer2_h
  #define _ArduinoTimer2_h

  class ArduinoTimer2
  {
    public:
      ArduinoTimer2(void);
      void setup(void);
      void start(unsigned char colour);
      unsigned long comparator;
      unsigned long prescaler;
    private:
      void setMode (const byte mode, const byte clock, const byte port);
  };
#endif
