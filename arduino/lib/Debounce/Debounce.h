#include "Arduino.h"

class Debounce {
  public:
    Debounce();

  protected:
    byte debounced_state(byte reading);
    int debounce_count;
    byte current_state;
    int counter;

};
