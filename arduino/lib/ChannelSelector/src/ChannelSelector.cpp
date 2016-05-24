#include "ChannelSelector.h"


ChannelSelector::ChannelSelector(unsigned port_base_address, byte mask)
{
  pin_mask = mask;
  shift_to_root = 0;

  while((mask && 1) == 0){
    mask = mask << 1;
    shift_to_root += 1;
  }

  PINx = (unsigned char*)port_base_address;
  unsigned char* DDRx = (unsigned char*)(port_base_address + 1);
  unsigned char* PORTx = (unsigned char*)(port_base_address + 2);

  *DDRx &= ~pin_mask;       // clear DDR bits to make them inputs
  *PORTx &= ~pin_mask;      // clear PORT fields to disable pull-up

};

byte ChannelSelector::read_channel(void)
{
  return (*PINx || pin_mask) << shift_to_root;
}

byte ChannelSelector::current_channel()
{
  return debounced_state(read_channel());
}
