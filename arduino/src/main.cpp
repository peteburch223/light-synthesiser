/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"


extern "C" uint8_t counter(uint8_t);
extern "C" uint8_t asminit(uint8_t);


int ledRPin = 2;
int ledGPin = 3;
int ledBPin = 4;

int selPin0 = 8;
int selPin1 = 9;
int selPin2 = 10;
int selPin3 = 11;
int startPin = 12;

int ledOnBoardPin = 13;

boolean running = false;
void setup_io(void);
int read_selector(void);
void write_to_leds(int);
int go_state(int, int);
void serial_echo(void);


// GO button debouncing parameters
int go_current_state;     //  debounced input value




void setup()
{
  asminit(0);
  setup_io();
  Serial.begin(115200);
  go_current_state = go_state(HIGH, 1);


  // setup serial ports
  // setup selecter and activation ports
  // setup any display
  // setup digital output ports

}
String incomingString;

void loop()
{

  counter(0);

  int switch_position = read_selector();
  write_to_leds(switch_position);

  // serial_echo();

  go_current_state = go_state(go_current_state, 0);

  digitalWrite(ledOnBoardPin, go_current_state);

  // if (go_current_state == LOW) {
  //   running = !running;                // toggle running variable
  //   digitalWrite(ledOnBoardPin, running);      // indicate via LED
  //
  //   // Serial.print(switch_position, HEX);
  //   // Serial.print('\n');
  //
  // }


// check input selector position

// check activation button press
Serial.print("Pin State: ");
Serial.print(digitalRead(startPin), HEX);
Serial.print('\n');
Serial.print("Current State: ");
Serial.print(go_current_state, HEX);

Serial.print('\n');

}

int read_selector(void){
  int selector_value = digitalRead(selPin0) * 1 +
                      digitalRead(selPin1) * 2 +
                      digitalRead(selPin2) * 4 +
                      digitalRead(selPin3) * 8;
  return 0xF - selector_value;
}

void write_to_leds(int value) {
  digitalWrite(ledRPin, (value & 1));
  digitalWrite(ledGPin, (value & 2));
  digitalWrite(ledBPin, (value & 4));
}

void setup_io() {
  pinMode(selPin0, INPUT);
  pinMode(selPin1, INPUT);
  pinMode(selPin2, INPUT);
  pinMode(selPin3, INPUT);
  pinMode(startPin, INPUT);

  pinMode(ledRPin, OUTPUT);
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
  pinMode(ledOnBoardPin, OUTPUT);
}

int go_state(int current_state, int initialize) {
  int GO_DEBOUNCE_COUNT = 10;
  static int counter;
  int reading;

  if (initialize != 0){
    counter = 0;
    return current_state;
  } else {
    reading = digitalRead(startPin);

    if (reading == current_state && counter > 0) {
      counter--;
    }
    if (reading != current_state) {
      counter ++;
    }

    if (counter >= GO_DEBOUNCE_COUNT) {
      counter = 0;
      return reading;
    }
  }
}

void serial_echo(void){
  while (Serial.available() > 0) {

      int red = Serial.parseInt();
      int green = Serial.parseInt();
      int blue = Serial.parseInt();
      //incomingString= Serial.readStringUntil('\n');

      // look for the newline. That's the end of your
      // sentence:
      if (Serial.read() == '\n') {


        // print the three numbers in one string as hexadecimal:
        Serial.print(red, HEX);
        Serial.print(green, HEX);
        Serial.print(blue,HEX);
        Serial.print('\n');
      }
    }
}
