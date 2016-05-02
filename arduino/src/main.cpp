
#include "Arduino.h"
#include <TimerHelpers.h>

const byte ledRPin = 2;
const byte ledGPin = 3;
const byte ledBPin = 4;

const byte ledOnBoardPin = 13;
const byte selPin0 = 14;
const byte selPin1 = 15;
const byte selPin2 = 16;
const byte selPin3 = 17;
const byte startPin = 18;

const byte SHUTTER = 9;  // this is OC1A (timer 1 output compare A)

// function declarations
void setup_io(void);
int read_selector(void);
void write_to_leds(int value);
int debounce(int current_state, int pin_value, int *counter, int initialize);
void serial_echo(void);
void serial_echo_line(void);
void readback_array(int);
void light_timer(long int duration, int pin);
void one_shot(int duration);

int go_current_state;
int channel_current_state;

int go_debounce_counter;
int channel_debounce_counter;

int value_array[16][3];
// boolean led_state = false;

boolean data_received;
boolean triggered;





void setup()
{
  Serial.begin(115200);
  setup_io();
  go_current_state = debounce(HIGH, 0, &go_debounce_counter, 1);
  channel_current_state = debounce(0, 0, &channel_debounce_counter, 1);
  data_received = false;
  triggered = false;
}

ISR(TIMER1_COMPA_vect)
{
   TCCR1A = 0;        // reset timer 1
   TCCR1B = 0;
   digitalWrite(ledRPin, LOW);
   triggered = false;
   Serial.println("Pulse Ended");

}

void loop()
{
  int go_previous_state = go_current_state;
  channel_current_state = debounce(channel_current_state, read_selector(),&channel_debounce_counter, 0);
  go_current_state = debounce(go_current_state, digitalRead(startPin), &go_debounce_counter, 0);


  if (go_current_state != go_previous_state) {
    // led_state = !led_state;
    if(go_current_state == LOW && !triggered){
      triggered = true;

      Serial.print("Triggered channel ");
      Serial.println(channel_current_state, HEX);


      one_shot(65535);

      // light_timer(200,ledRPin);
      // light_timer(300,ledGPin);
      // light_timer(100,ledBPin);

      // write_to_leds(channel_current_state);
    }
  }

  // serial_echo_line();
  // if(data_received){
  //   readback_array(channel_current_state);
  // }
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



void one_shot(int duration) {
  // delay (250);   // debugging

  TCCR1A = 0;        // reset timer 1
  TCCR1B = 0;

  digitalWrite(ledRPin, HIGH);;   // ready to activate

  // set up Timer 1
  TCNT1 = 0;         // reset counter
  OCR1A =  duration;       // compare A register value (1000 * clock speed)

  // Mode 4: CTC, top = OCR1A
  Timer1::setMode (4, Timer1::PRESCALE_1024, Timer1::NO_PORT);

  TIFR1 |= bit (OCF1A);    // clear interrupt flag
  TIMSK1 = bit (OCIE1A);   // interrupt on Compare A Match
}


void light_timer(long int duration, int pin) {
  // noInterrupts();
  digitalWrite(pin, HIGH);

  while(duration > 0){
    // Serial.println(duration,HEX);
    delay(5);
    duration--;
  }
  digitalWrite(pin, LOW);
  // interrupts();
}

int debounce(int current_state, int reading, int *counter, int initialize) {
  int DEBOUNCE_COUNT = 10;

  if (initialize != 0){
    *counter = 0;
    return current_state;
  } else {
    if (*counter >= DEBOUNCE_COUNT) {
      *counter = 0;
      return reading;
    } else {
      if (reading == current_state && *counter > 0) {
        (*counter)--;
      }
      if (reading != current_state) {
        (*counter)++;
      }
      return current_state;
    }
  }
}

void readback_array(int channel) {
  Serial.println("START OF READBACK");
  Serial.print("Channel Selected:");
  Serial.print(channel, HEX);
  Serial.print('\n');

  for(int i = 0; i<16; i++){
    Serial.print(value_array[i][0], HEX);
    Serial.print(value_array[i][1], HEX);
    Serial.print(value_array[i][2], HEX);
    Serial.print('\n');
    }
  Serial.println("END OF READBACK");
  data_received = false;

}


void serial_echo_line(void){
  static int arrayPointer;

  if (Serial.available()) {

      if (Serial.peek() == 'z'){
        Serial.readStringUntil('\n');
        Serial.print("START OF BLOCK");
        Serial.print('\n');
        arrayPointer = 0;

      } else if (Serial.peek() == 'x') {
        Serial.readStringUntil('\n');
        Serial.print("END OF BLOCK");
        Serial.print('\n');
        data_received = true;

      } else {
        int red = Serial.parseInt();
        int green = Serial.parseInt();
        int blue = Serial.parseInt();
        if (Serial.read() == '\n') {
          value_array[arrayPointer][0] = red;
          value_array[arrayPointer][1] = green;
          value_array[arrayPointer][2] = blue;

          Serial.print(red, HEX);
          Serial.print(green, HEX);
          Serial.print(blue,HEX);
          Serial.print("OK");
          Serial.print('\n');

          arrayPointer += 1;

      }
    }
  }
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
  pinMode (SHUTTER, OUTPUT);

  digitalWrite(ledRPin, LOW);
  digitalWrite(ledGPin, LOW);
  digitalWrite(ledBPin, LOW);
  digitalWrite(ledOnBoardPin, LOW);
  digitalWrite (SHUTTER, LOW);
}
