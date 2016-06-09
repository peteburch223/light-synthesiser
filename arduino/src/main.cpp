/*
 Name:		main.cpp
 Created:	6/9/2016 10:19:27 AM
 Author:	Pete
*/
#include <Arduino.h>
#include "ReadPortValue.h"
#include "SequenceState.h"


#define VALUE_ARRAY_SIZE 16
#define SELECTOR_PIN_MASK 0x0F
#define SELECTOR_DEFAULT 0x00
#define TRIGGER_PIN_MASK 0x20
#define TRIGGER_DEFAULT 0x00
#define PINC_ADDRESS 0x26;

struct Duration {
	unsigned long duration;
	unsigned long t2_prescaler_pointer;
	unsigned long t2_comparator;
	unsigned long t1_comparator;
};




boolean dataReceived;
Duration value_array[VALUE_ARRAY_SIZE][3];
unsigned char * pinc = (unsigned char *) PINC_ADDRESS;

ReadPortValue selector(SELECTOR_DEFAULT, pinc, SELECTOR_PIN_MASK);
ReadPortValue trigger(TRIGGER_DEFAULT, pinc, TRIGGER_PIN_MASK);
SequenceState stateMachine;
// TimerController timer;
boolean timerComplete;

// Function declarations - these will be moved to objects

void serial_echo_line(void);
void readback_array(int channel);
void initialize_value_array(void);
void debugSerial(void);





// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	// setup_io();
	dataReceived = false;
	timerComplete = false;
	initialize_value_array();
}

ISR(TIMER1_COMPA_vect)
{
  TCCR1A = 0;               // reset timer 2
  TCCR1B = 0;
	timerComplete = true;
}


// the loop function runs over and over again until power down or reset
void loop() {

	// timer.checkForStart(stateMachine, selector.valueShiftedToRoot());

	stateMachine.buttonState = trigger.valueShiftedToRoot();
	stateMachine.timerComplete = timerComplete;
	stateMachine.advance();



	serial_echo_line();
	if (dataReceived) {
		readback_array(0);
	}

debugSerial();
	delay(100);
}

void debugSerial(void)
{
	Serial.print("State: ");
	Serial.println(stateMachine.state, HEX);
	Serial.print("Colour: ");
	Serial.println(stateMachine.colour, HEX);


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

void readback_array(int channel) {
	Serial.println("START OF READBACK");
	Serial.print("Channel Selected:");
	Serial.print(channel, HEX);
	Serial.print('\n');

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
