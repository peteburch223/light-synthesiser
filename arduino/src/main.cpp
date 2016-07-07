/*
 Name:		main.cpp
 Created:	6/9/2016 10:19:27 AM
 Author:	Pete
*/
#include <Arduino.h>
#include "ReadPortValue.h"
#include "SequenceState.h"
#include "Duration.h"
#include "TimerController.h"
#include "ArduinoTimer1.h"
#include "ArduinoTimer2.h"

#define BAUD_RATE 115200
#define VALUE_ARRAY_SIZE 16
#define SELECTOR_PIN_MASK 0x0F
#define SELECTOR_DEFAULT 0x00
#define TRIGGER_PIN_MASK 0x20
#define TRIGGER_DEFAULT 0x00
#define PINC_ADDRESS 0x26;

bool dataReceived;
Duration value_array[VALUE_ARRAY_SIZE][3];

Duration durations;
unsigned char * pinc = (unsigned char *) PINC_ADDRESS;

ReadPortValue selector(SELECTOR_DEFAULT, pinc, SELECTOR_PIN_MASK);
ReadPortValue trigger(TRIGGER_DEFAULT, pinc, TRIGGER_PIN_MASK);
SequenceState stateMachine;
ArduinoTimer1 timer1;
ArduinoTimer2 timer2;
TimerController timer(&timer1, &timer2, &stateMachine, value_array);
bool timerComplete;

// Function declarations - these will be moved to objects

void serial_echo_line(void);
void readback_array(void);
void initialize_value_array(void);
void debugSerial(void);





// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(BAUD_RATE);
	// setup_io();
	dataReceived = false;
	timerComplete = false;
	initialize_value_array();
	readback_array();
}

ISR(TIMER1_COMPA_vect)
{
	PIND &= 0xE3;		//Clear colour output pins
  TCCR1A = 0x00;
  TCCR1B = 0x00;
	TCCR2A = 0x00;
	TCCR2B = 0x00;
	stateMachine.setTimerComplete();
  Serial.println("T1 interrupt");
}


// the loop function runs over and over again until power down or reset
void loop() {

	timer.checkForStart(selector.valueShiftedToRoot());

	stateMachine.buttonState = trigger.valueShiftedToRoot();
	stateMachine.advance();

	serial_echo_line();
	if (dataReceived) {
		readback_array();
	}

	// debugSerial();
	delay(100);
}

void debugSerial(void)
{



	unsigned char temp;

	// Serial.print("PINC: ");
	// Serial.println(PINC, HEX);
	//
	// Serial.print("DDRC: ");
	// Serial.println(DDRC, HEX);
	//
	// Serial.print("PORTC: ");
	// Serial.println(PORTC, HEX);
	temp = selector.valueShiftedToRoot();
	Serial.print("Selector: ");
	Serial.println(temp, HEX);

	temp = trigger.valueShiftedToRoot();
	Serial.print("Trigger: ");
	Serial.println(temp, HEX);
}


void serial_echo_line(void) {
	static int arrayPointer;

	if (Serial.available()) {

		if (Serial.peek() == 'z') {
			Serial.readStringUntil('\n');
			Serial.print("START OF BLOCK");
			Serial.print('\n');
			arrayPointer = 0;

		}
		else if (Serial.peek() == 'x') {
			Serial.readStringUntil('\n');
			Serial.print("END OF BLOCK");
			Serial.print('\n');
			dataReceived = true;

		}
		else {
			unsigned long red = Serial.parseInt();
			unsigned long  green = Serial.parseInt();
			unsigned long  blue = Serial.parseInt();
			if (Serial.read() == '\n') {
				value_array[arrayPointer][0].duration = red;
				value_array[arrayPointer][1].duration = green;
				value_array[arrayPointer][2].duration = blue;

				Serial.print(red, HEX);
				Serial.print(green, HEX);
				Serial.print(blue, HEX);
				Serial.print("OK");
				Serial.print('\n');

				if (arrayPointer < VALUE_ARRAY_SIZE) {
					arrayPointer += 1;
				}
			}
		}
	}
}

void readback_array(void) {
	Serial.println("START OF READBACK");
	// Serial.print("Channel Selected:");
	// Serial.print(channel, HEX);
	// Serial.print('\n');

	for (int i = 0; i<16; i++) {
		Serial.print(value_array[i][0].duration, HEX);
		Serial.print(value_array[i][1].duration, HEX);
		Serial.print(value_array[i][2].duration, HEX);
		Serial.print('\n');
	}
	Serial.println("END OF READBACK");
	dataReceived = false;

}

void initialize_value_array(void) {
	for (int i = 0; i<16; i++) {
		for (int j = 0; j<3; j++) {
			int channel = i;
			int colour = j << 4;
			value_array[i][j].duration = 0xFFFFFF00 | colour | channel;
		}
	}
}

// void setup_io(void) {
//   pinMode(SEL_PIN_0, INPUT);
//   pinMode(SEL_PIN_1, INPUT);
//   pinMode(SEL_PIN_2, INPUT);
//   pinMode(SEL_PIN_3, INPUT);
//   pinMode(START_PIN, INPUT);
//
//   pinMode(GATE_PIN, OUTPUT);
//   pinMode(RED_PIN, OUTPUT);
//   pinMode(GREEN_PIN, OUTPUT);
//   pinMode(BLUE_PIN, OUTPUT);
//   pinMode(TIMER0_CLOCK_PIN, OUTPUT);
//
//   digitalWrite(GATE_PIN, LOW);
//   digitalWrite(RED_PIN, LOW);
//   digitalWrite(GREEN_PIN, LOW);
//   digitalWrite(BLUE_PIN, LOW);
//   digitalWrite(TIMER0_CLOCK_PIN, LOW);
// }
