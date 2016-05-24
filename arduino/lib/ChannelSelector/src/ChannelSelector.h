#include <Debounce.h>
#include "Arduino.h"

class ChannelSelector: public Debounce
{
  public:
    byte current_channel(void);
    ChannelSelector(unsigned  port_base_address, byte pin_mask);

  private:
    byte read_channel(void);
    unsigned char* PINx;
    byte pin_mask;
    byte shift_to_root;
};
