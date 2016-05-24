
#include "Arduino.h"
#include <TimerHelpers.h>
#include "ChannelSelector.cpp"

const int VALUE_ARRAY_SIZE = 16;
const int WAIT_COUNT = 0x7F;


const byte GATE_PIN = 3;
const byte RED_PIN = 5;
const byte GREEN_PIN = 6;
const byte BLUE_PIN = 7;
const byte LED_ON_MASK_BASE = 0x20;
const byte LED_OFF_MASK = 0x1F;

const byte TIMER0_CLOCK_PIN = 9;

const byte SEL_PIN_0 = 14;
const byte SEL_PIN_1 = 15;
const byte SEL_PIN_2 = 16;
const byte SEL_PIN_3 = 17;
const byte START_PIN = 19;

// function declarations
void setup_io(void);
int read_selector(void);
int debounce(int current_state, int pin_value, int *counter, int initialize);
void serial_echo_line(void);
void readback_array(int);
void timer_0_one_shot(int duration);
void timer_1_free_run();
void led_sequence();
void check_for_trigger();
void wait_state();
void timer_2_start_wait();

void initialize_globals();
void initialize_value_array();

ChannelSelector channel(0x08, 0xF);


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
boolean start_wait;
boolean wait;
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
  start_wait = true;
  colour_pointer += 1;      // increment colour pointer
}

ISR(TIMER2_COMPA_vect)
{
  TCCR2A = 0;               // reset timer 2
  TCCR2B = 0;
  wait = false;
  Serial.println("Wait completed");
}

void loop()
{
  go_previous_state = go_current_state;
  channel_current_state = debounce(channel_current_state, read_selector(),&channel_debounce_counter, 0);
  go_current_state = debounce(go_current_state, digitalRead(START_PIN), &go_debounce_counter, 0);

  channel_current_state = channel.current_channel();

  check_for_trigger();
  wait_state();
  led_sequence();

  serial_echo_line();
  if(data_received){
    readback_array(channel_current_state);
  }
}

int read_selector(void){
  int selector_value = digitalRead(SEL_PIN_0) * 1 +
                      digitalRead(SEL_PIN_1) * 2 +
                      digitalRead(SEL_PIN_2) * 4 +
                      digitalRead(SEL_PIN_3) * 8;
  return 0xF - selector_value;
}

void check_for_trigger(){

  if (go_current_state != go_previous_state) {
    if(go_current_state == LOW && !sequence_triggered){
      digitalWrite(GATE_PIN, HIGH);

      sequence_triggered = true;
      start_wait = true;
      selected_channel = channel_current_state;
      colour_pointer = 0;

      Serial.print("sequence_triggered channel ");
      Serial.println(channel_current_state, HEX);
    }
  }
}

void led_sequence(){

  if (sequence_triggered && !timer_running && !wait){

    if (colour_pointer < 3){
      Serial.print("Colour ");
      Serial.println(colour_pointer, HEX);
      Serial.print("Duration ");
      Serial.println(value_array[selected_channel][colour_pointer], HEX);

      timer_0_one_shot(0x7F);

    } else {
      sequence_triggered = false;
      digitalWrite(GATE_PIN, LOW);

      wait = false;
      colour_pointer = 0;
      Serial.println("Sequence completed");
    }
  }
}

void wait_state(){
  if(start_wait) {
    Serial.println("Starting wait");
    timer_2_start_wait();
    wait = true;
    start_wait = false;
  }
}

void timer_1_free_run(){

  TCCR1A = 0;        // reset timer 1
  TCCR1B = 0;
  OCR1A = 0xFF;
  TCNT1 = 0;         // reset counter
  Timer1::setMode (4, Timer1::PRESCALE_64, Timer1::TOGGLE_A_ON_COMPARE);
}

void timer_0_one_shot(int duration) {

  int led_mask = LED_ON_MASK_BASE << colour_pointer;

  TCCR0A = 0;        // reset timer 1
  TCCR0B = 0;        // clocked on rising edge of external clock

  timer_running = true;

  PORTD |= led_mask;


  // set up Timer 0
  TCNT0 = 0;         // reset counter
  OCR0A =  duration;       // compare A register value (1000 * clock speed)

  // Mode 2: CTC, top = OCR0A
  Timer0::setMode (2, Timer0::T0_RISING, Timer0::NO_PORT);

  TIFR0 |= bit (OCF0A);    // clear interrupt flag
  TIMSK0 = bit (OCIE0A);   // interrupt on Compare A Match
}

void timer_2_start_wait() {

  TCCR2A = 0;        // reset timer 1
  TCCR2B = 0;        // clocked on rising edge of external clock

  TCNT2 = 2;         // reset counter
  OCR2A =  WAIT_COUNT;

  Timer2::setMode (2, Timer2::PRESCALE_1024, Timer0::NO_PORT);

  TIFR2 |= bit (OCF2A);    // clear interrupt flag
  TIMSK2 = bit (OCIE2A);   // interrupt on Compare A Match
}

// int timer0_compare_value(int duration) {
//
//
// }
//
// int timer1_prescaler_value(int duration) {
//
//
//
//   return duration
// }
//
// int timer1_compare_value(int duration) {
//
// }

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
  pinMode(SEL_PIN_0, INPUT);
  pinMode(SEL_PIN_1, INPUT);
  pinMode(SEL_PIN_2, INPUT);
  pinMode(SEL_PIN_3, INPUT);
  pinMode(START_PIN, INPUT);

  pinMode(GATE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(TIMER0_CLOCK_PIN, OUTPUT);

  digitalWrite(GATE_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(TIMER0_CLOCK_PIN, LOW);
}

void initialize_globals(){
  go_current_state = debounce(HIGH, 0, &go_debounce_counter, 1);
  channel_current_state = debounce(0, 0, &channel_debounce_counter, 1);
  data_received = false;
  sequence_triggered = false;
  timer_running = false;
  start_wait = false;
  wait = false;
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
