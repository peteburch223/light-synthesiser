/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"



void setup()
{
  Serial.begin(115200);
  asminit(0);


  // setup serial ports
  // setup selecter and activation ports
  // setup any display
  // setup digital output ports

}
String incomingString;

void loop()
{

  counter(0);

//  Serial.println("Test");
// check serial buffer for data - if there is any load into data structure
//
// //send data only when you receive data:
// if (Serial.available() > 0) {
//         // read the incoming byte:
//         incomingString = Serial.readStringUntil('\n');
//
//         // say what you got:
//         Serial.print("I received: ");
//         Serial.println(incomingString);
//       }
digitalWrite(13, LOW);
while (Serial.available() > 0) {
  digitalWrite(13, HIGH);

    // look for the next valid integer in the incoming serial stream:
    int red = Serial.parseInt();
    // do it again:
    int green = Serial.parseInt();
    // do it again:
    int blue = Serial.parseInt();
    //incomingString= Serial.readStringUntil('\n');

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {

      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      // red = 255 - constrain(red, 0, 255);
      // green = 255 - constrain(green, 0, 255);
      //blue = 255 - constrain(blue, 0, 255);

      // print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.print(blue,HEX);
      Serial.print('\n');
    }
  }

// check input selector position

// check activation button press

//delay(10);

}

void asm_nit(void) __attribute__((naked))
{
 asm volatile ("
      sbi  4,5; 4 = DDRB (0x24 - 0x20). Bit 5 = pin 13
 ");
}
//void counter(void) __attribute__((naked))
//{
// asm volatile ("
//
//      ldi r18,0x08   ; LSByte
//      ldi r19,0x00   ;
//      ldi r25,0x00   ; MSByte
//      sbi 5, 5       ; 5 = PORTB (0x25 - 0x20). Bit 5 = pin 13
//dl1:  subi r18,1     ;
//      sbci r19,0     ;
//      sbci r25,0     ;
//      brne dl1       ;
//      cbi 5, 5       ; 5 = PORTB (0x25 - 0x20). Bit 5 = pin 13
//  ");
//}


