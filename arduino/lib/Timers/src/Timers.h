#include "Arduino.h"
#include "TimerHelpers.h"

#ifndef _ArduinoTimers_h
  #define _ArduinoTimers_h

  class ArduinoTimer1
  {
    public:
      ArduinoTimer1(void);
      void setup(void);
      void start(void);
      unsigned long comparator;
  };

  class ArduinoTimer2
  {
    public:
      ArduinoTimer2(void);
      void setup(void);
      void start(void);
      unsigned long comparator;
      unsigned long prescaler;
  };

#endif
