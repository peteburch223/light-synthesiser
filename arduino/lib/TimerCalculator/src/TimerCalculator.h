#ifndef _TimerCalculator_h
  #define _TimerCalculator_h

  class TimerCalculator {
    public:
      TimerCalculator(void);
      void calculate(unsigned long duration);
      unsigned long t2_prescaler_pointer;
      unsigned long t2_comparator;
      unsigned long t1_comparator;

    protected:
      unsigned long clock_period;
      unsigned long _duration;
      unsigned long t2_prescaler_array[8];
      unsigned long calc_t2_period(void);
      void debugSerial(void);
  };

#endif
