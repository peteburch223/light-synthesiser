#include "TimerCalculator.h"
#include "arduino.h"

// ALL DURATION VALUES ARE IN CLOCK CYCLES

// #define CLOCK_PERIOD 125 // ns
#define MAX_T2_COMPARATOR 0xFF
#define MAX_T1_COMPARATOR 0xFFFF
#define MAX_T2_PRESCALER_POINTER 6

TimerCalculator::TimerCalculator() :
  t2_prescaler_array{1, 8, 32, 64, 128, 256, 1024}
{
}

void TimerCalculator::calculate(unsigned long duration)
{
  _duration = duration;
  t2_prescaler_pointer = 0;
  t2_comparator = 1;

  while (true)
  {

    // if duration can be achieved with T2 parameters then stop
    if (calcDuration(MAX_T1_COMPARATOR) > duration)
    {
      Serial.println("...duration achievable - break");
      break;
    }

    if (t2_prescaler_pointer == MAX_T2_PRESCALER_POINTER)
    {
      if (t2_comparator == MAX_T2_COMPARATOR)
      {
        Serial.println("...max T2 comparator - break");      
        break;
      }
      else
      {
        t2_comparator += 1;
      }
    }
    else
    {
      if (t2_comparator < (t2_prescaler_array[t2_prescaler_pointer + 1] / t2_prescaler_array[t2_prescaler_pointer]))
      {
        t2_comparator += 1;
      } else {
        t2_comparator = 1;
        t2_prescaler_pointer += 1;
      }
    }

    debug_data();
  }

  t1_comparator = (unsigned long)(duration / calc_t2_period());

  Serial.println("*****************");
  Serial.print("t1_comparator: ");
  Serial.println(t1_comparator, HEX);
  Serial.println("*****************");
}

unsigned long TimerCalculator::calcDuration(unsigned long t1_comparator_value)
{
  unsigned long t2_period = calc_t2_period() * t2_comparator;
  return t1_comparator_value * t2_period;
}

unsigned long TimerCalculator::calc_t2_period(void)
{
  unsigned long prescaler = t2_prescaler_array[t2_prescaler_pointer];
  return prescaler * t2_comparator;
}

void TimerCalculator::debug_data(void)
{
  Serial.println("*****************");
  Serial.print("duration: ");
  Serial.println(_duration);

  Serial.print("max duration: ");
  Serial.println(calcDuration(MAX_T1_COMPARATOR));
  Serial.print("t2_prescaler_pointer: ");
  Serial.println(t2_prescaler_pointer, HEX);
  Serial.print("t2_comparator: ");
  Serial.println(t2_comparator, HEX);

  delay(10);
}
