
#include "Arduino.h"
#include <TimerHelpers.h>

// const byte ledRPin = 2;
// const byte ledGPin = 3;
// const byte ledBPin = 4;
// const byte LED_ON_MASK_BASE = 0x04;
// const byte LED_OFF_MASK = 0xE3;

const int VALUE_ARRAY_SIZE = 16;

const byte ledRPin = 5;
const byte ledGPin = 6;
const byte ledBPin = 7;
const byte LED_ON_MASK_BASE = 0x20;
const byte LED_OFF_MASK = 0x1F;

const byte TIMER0_CLOCK_PIN = 9;


const byte ledOnBoardPin = 13;
const byte selPin0 = 14;
const byte selPin1 = 15;
const byte selPin2 = 16;
const byte selPin3 = 17;
const byte startPin = 19;

// function declarations
void setup_io(void);
int read_selector(void);
// void write_to_leds(int value);
int debounce(int current_state, int pin_value, int *counter, int initialize);
// void serial_echo(void);
void serial_echo_line(void);
void readback_array(int);
// void light_timer(long int duration, int pin);
void timer_1_one_shot();
void timer_0_one_shot();
void timer_1_free_run();
void led_sequence();
void check_for_trigger();

void initialize_globals();
void initialize_value_array();


int go_previous_state;
int go_current_state;
int channel_current_state;

int go_debounce_counter;
int channel_debounce_counter;

int selected_channel;

int value_array[VALUE_ARRAY_SIZE][3];


boolean data_received;
boolean sequence_triggered;
boolean timer_running;
byte colour_pointer;





void setup()
{
  Serial.begin(115200);
  setup_io();
  initialize_value_array();
  initialize_globals();
  timer_1_free_run();
}

ISR(TIMER0_COMPA_vect)
{
  PORTD &= LED_OFF_MASK;    // clear all LED pins
  TCCR0A = 0;               // reset timer 0
  TCCR0B = 0;
  timer_running = false;
  colour_pointer += 1;      // increment colour pointer
}

void loop()
{
  go_previous_state = go_current_state;
  channel_current_state = debounce(channel_current_state, read_selector(),&channel_debounce_counter, 0);
  go_current_state = debounce(go_current_state, digitalRead(startPin), &go_debounce_counter, 0);


  check_for_trigger();
  led_sequence();

  serial_echo_line();
  if(data_received){
    readback_array(channel_current_state);
  }
}

int read_selector(void){
  int selector_value = digitalRead(selPin0) * 1 +
                      digitalRead(selPin1) * 2 +
                      digitalRead(selPin2) * 4 +
                      digitalRead(selPin3) * 8;
  return 0xF - selector_value;
}

void check_for_trigger(){

  if (go_current_state != go_previous_state) {
    if(go_current_state == LOW && !sequence_triggered){
      sequence_triggered = true;
      selected_channel = channel_current_state;
      colour_pointer = 0;

      Serial.print("sequence_triggered channel ");
      Serial.println(channel_current_state, HEX);
    }
  }
}



void led_sequence(){

  if (sequence_triggered && !timer_running){
    if (colour_pointer < 3){
      Serial.print("Colour ");
      Serial.println(colour_pointer, HEX);
      Serial.print("Duration ");
      Serial.println(value_array[selected_channel][colour_pointer], HEX);

      timer_0_one_shot();
      // one_shot();
    } else {
      sequence_triggered = false;
      colour_pointer = 0;
      Serial.println("Sequence completed");

    }
  }
}

void timer_1_free_run(){

  TCCR1A = 0;        // reset timer 1
  TCCR1B = 0;

  // set up Timer 1
  OCR1A = 0xF;
  TCNT1 = 0;         // reset counter

  // Mode 4: CTC, top = OCR1A
  // Toggle Pin 9
  Timer1::setMode (4, Timer1::PRESCALE_1, Timer1::TOGGLE_A_ON_COMPARE);
}




void timer_0_one_shot() {
  // delay (250);   // debugging

  int duration = 0xFF;
  int led_mask = LED_ON_MASK_BASE << colour_pointer;

  TCCR0A = 0;        // reset timer 1
  TCCR0B = 0;        // clocked on rising edge of external clock

  timer_running = true;
  PORTD |= led_mask;

  // set up Timer 0
  TCNT0 = 0;         // reset counter
  OCR0A =  duration;       // compare A register value (1000 * clock speed)

  // Mode 2: CTC, top = OCR0A
  // *** NEED TO FIGURE OUT HOW TO CONFIGURE FOR EXTERNAL CLOCK ON PIN D4
  Timer0::setMode (2, Timer0::T0_RISING, Timer0::NO_PORT);

  TIFR0 |= bit (OCF0A);    // clear interrupt flag
  TIMSK0 = bit (OCIE0A);   // interrupt on Compare A Match
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

        if (arrayPointer < VALUE_ARRAY_SIZE){
          arrayPointer += 1;
        }
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
  pinMode(TIMER0_CLOCK_PIN, OUTPUT);

  pinMode(ledOnBoardPin, OUTPUT);

  digitalWrite(ledRPin, LOW);
  digitalWrite(ledGPin, LOW);
  digitalWrite(ledBPin, LOW);
  digitalWrite(ledOnBoardPin, LOW);
  digitalWrite(TIMER0_CLOCK_PIN, LOW);
}

void initialize_globals(){
  go_current_state = debounce(HIGH, 0, &go_debounce_counter, 1);
  channel_current_state = debounce(0, 0, &channel_debounce_counter, 1);
  data_received = false;
  sequence_triggered = false;
  timer_running = false;
  colour_pointer = 0;
}


void initialize_value_array() {
  for(int i=0; i<16; i++){
    for(int j=0; j<3; j++){
      int channel = i;
      int colour = j << 4;
      value_array[i][j] = 0xFFFFFF00 | colour | channel;
    }
  }
}


// UNUSED FUNCTIONS

void write_to_leds(int value) {
  digitalWrite(ledRPin, (value & 1));
  digitalWrite(ledGPin, (value & 2));
  digitalWrite(ledBPin, (value & 4));
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
