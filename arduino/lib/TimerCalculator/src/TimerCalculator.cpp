#include "TimerCalculator.h"
#include "arduino.h"

// ALL DURATION VALUES ARE IN CLOCK CYCLES

// #define CLOCK_PERIOD 125 // ns
#define MAX_T2_COMPARATOR 255

TimerCalculator::TimerCalculator() :
  t2_prescaler_array{1, 8, 32, 64, 128, 256, 1024}
{
}

void TimerCalculator::calculate(unsigned long duration)
{
  // readback_array();
  _duration = duration;
  t2_prescaler_pointer = 0;
  t2_comparator = 1;
  t1_comparator = 1;
  // clock_period = CLOCK_PERIOD;
  unsigned long max_t1_comparator = 65535;
  unsigned long max_duration = calc_period(max_t1_comparator);
  //Serial.print("duration: ");
  //Serial.println(_duration);
  //Serial.print("max_duration: ");
  //Serial.println(max_duration);

  while (duration > max_duration )
  {

    // Serial.print("duration: ");
    // Serial.println(_duration);
    // Serial.print("max_duration: ");
    // Serial.println(max_duration);

    if (t2_comparator < (t2_prescaler_array[t2_prescaler_pointer + 1] / t2_prescaler_array[t2_prescaler_pointer]))
    {
      t2_comparator += 1;

    } else {
      t2_comparator = 1;
      t2_prescaler_pointer += 1;
    }
    max_duration = calc_period(max_t1_comparator);
    if(t2_comparator == MAX_T2_COMPARATOR) break;
    debug_data();
  }

  // Serial.print("calc_t2_period: ");
  // Serial.println(calc_t2_period());
  // Serial.print("result: ");
  // Serial.println(duration / calc_t2_period());
  t1_comparator = (int)(duration / calc_t2_period());
}

unsigned long TimerCalculator::calc_period(unsigned long t1_comparator_value)
{
  unsigned long t2_period = calc_t2_period() * t2_comparator;
  return t1_comparator_value * t2_period;
}

unsigned long TimerCalculator::calc_t2_period(void)
{
  // unsigned long prescaler = clock_period * t2_prescaler_array[t2_prescaler_pointer];
  unsigned long prescaler = t2_prescaler_array[t2_prescaler_pointer];
  return prescaler * t2_comparator;
}
void TimerCalculator::debug_data(void)
{
  // Serial.println("*****************");
  // Serial.print("t1_comparator: ");
  // Serial.println(t1_comparator);
  // Serial.print("t2_prescaler: ");
  // Serial.println(t2_prescaler_array[t2_prescaler_pointer]);
  // Serial.print("t2_comparator: ");
  // Serial.println(t2_comparator);
  // Serial.println("*****************");
}

// void TimerCalculator::readback_array(void) {
//   Serial.println("START OF READBACK");
//
//   for(int i = 0; i<7; i++){
//     Serial.println(t2_prescaler_array[i], HEX);
//     }
//   Serial.println("END OF READBACK");
// }
