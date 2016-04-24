#include "avr/io.h"

; Define the function asminit()
.global asminit
asminit:
      sbi  4,5; 4 = DDRB (0x24 - 0x20). Bit 5 = pin 13
      ret

.global counter
counter:
      ldi r18,0x08	 ; LSByte
      ldi r19,0x00	 ;
      ldi r25,0x00	 ; MSByte
      sbi 5, 5	 ; 5 = PORTB (0x25 - 0x20). Bit 5 = pin 13
dl1:	subi r18,1	 ;
      sbci r19,0	 ;
      sbci r25,0	 ;
      brne dl1
      cbi 5, 5	 ; 5 = PORTB (0x25 - 0x20). Bit 5 = pin 13
      ret
