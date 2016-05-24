#include "Debounce.h"

Debounce::Debounce(void)
{
  counter = 0;
  debounce_count = 10;
};

byte Debounce::debounced_state(byte reading)
{
  if (counter >= debounce_count) {
    counter = 0;
    return reading;
  } else {
    if (reading == current_state && counter > 0) {
      (counter)--;
    }
    if (reading != current_state) {
      (counter)++;
    }
    return current_state;
  }
}
