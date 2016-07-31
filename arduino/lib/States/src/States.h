#ifndef _States_h
  #define _States_h

  enum { AWAIT_COMMAND, WAIT, DISPLAY_COLOUR, TEST };
  enum { RED, GREEN, BLUE, NO_COLOUR};

  #define DEBUG false
  #define BAUD_RATE 115200
  #define VALUE_ARRAY_SIZE 32
#endif
