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
#include "TimerCalculator.h"
#include "States.h"

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
void readbackCalculatedValues(void);
// void initialize_value_array(void);
void debugSerial(void);
void initializeValueArray(void);





// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(BAUD_RATE);
	dataReceived = false;
	timerComplete = false;
	initializeValueArray();
	// readback_array();
	// readbackCalculatedValues();
}

ISR(TIMER1_COMPA_vect)
{
	PORTD &= 0xE3;		// Clear colour output pins
	TIMSK1 = 0x00; 		// Disable any further interrupts
	TCNT1 = 0x0000;
	TCNT2 = 0x00;
  TCCR1A = 0x00;		// Disable Timer 1
	TCCR1B = 0x00;		// Disable Timer 1
	TCCR2A = 0x00;		// Disable Timer 2
	TCCR2B = 0x00;		// Disable Timer 2
	stateMachine.setTimerComplete();
  if (DEBUG) Serial.println("T1 interrupt");
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
	// delay(100);
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

void readbackCalculatedValues(void) {
	Serial.println("START OF CALCULATED VALUES READBACK");
	// Serial.print("Channel Selected:");
	// Serial.print(channel, HEX);
	// Serial.print('\n');

	for (int i = 0; i<16; i++) {
		Serial.print("CHANNEL: ");
		Serial.println(i, HEX);
		Serial.println("Duration:");
		Serial.print(value_array[i][0].duration, HEX);
		Serial.print(",");
		Serial.print(value_array[i][1].duration, HEX);
		Serial.print(",");
		Serial.println(value_array[i][2].duration, HEX);

		Serial.println("t1_comparator:");
		Serial.print(value_array[i][0].t1_comparator, HEX);
		Serial.print(",");
		Serial.print(value_array[i][1].t1_comparator, HEX);
		Serial.print(",");
		Serial.println(value_array[i][2].t1_comparator, HEX);

		Serial.println("t2_comparator:");
		Serial.print(value_array[i][0].t2_comparator, HEX);
		Serial.print(",");
		Serial.print(value_array[i][1].t2_comparator, HEX);
		Serial.print(",");
		Serial.println(value_array[i][2].t2_comparator, HEX);

		Serial.println("t2_prescaler_pointer:");
		Serial.print(value_array[i][0].t2_prescaler_pointer, HEX);
		Serial.print(",");
		Serial.print(value_array[i][1].t2_prescaler_pointer, HEX);
		Serial.print(",");
		Serial.println(value_array[i][2].t2_prescaler_pointer, HEX);
	}
	Serial.println("END OF CALCULATED VALUES READBACK");
	dataReceived = false;
}

// void initialize_value_array(void) {
// 	TimerCalculator timerCalc;
// 	for (int i = 0; i<16; i++) {
// 		for (int j = 0; j<3; j++) {
// 			int channel = i;
// 			int colour = j << 4;
// 			value_array[i][j].duration = 0xFFF00 | colour | channel;
//
// 			timerCalc.calculate(value_array[i][j].duration);
// 			value_array[i][j].t1_comparator = timerCalc.t1_comparator;
// 			value_array[i][j].t2_comparator = timerCalc.t2_comparator;
// 			value_array[i][j].t2_prescaler_pointer = timerCalc.t2_prescaler_pointer;
// 		}
// 	}
// }

void initializeValueArray(void)
{
	TimerCalculator timerCalc;



	value_array[0][0].duration = 4200;
	value_array[0][1].duration = 5800;
	value_array[0][2].duration = 6600;

	value_array[1][0].duration = 7400;
	value_array[1][1].duration = 13200;
	value_array[1][2].duration = 19800;

	value_array[2][0].duration = 69800;
	value_array[2][1].duration = 120000;
	value_array[2][2].duration = 170000;

	value_array[3][0].duration = 50000;
	value_array[3][1].duration = 100000;
	value_array[3][2].duration = 200000;

	value_array[4][0].duration = 100000;
	value_array[4][1].duration = 200000;
	value_array[4][2].duration = 50000;

	value_array[5][0].duration = 100000;
	value_array[5][1].duration = 200000;
	value_array[5][2].duration = 50000;

	value_array[6][0].duration = 100000;
	value_array[6][1].duration = 200000;
	value_array[6][2].duration = 50000;

	value_array[7][0].duration = 100000;
	value_array[7][1].duration = 200000;
	value_array[7][2].duration = 50000;

	value_array[8][0].duration = 100000;
	value_array[8][1].duration = 200000;
	value_array[8][2].duration = 50000;

	value_array[9][0].duration = 100000;
	value_array[9][1].duration = 200000;
	value_array[9][2].duration = 50000;

	value_array[10][0].duration = 100000;
	value_array[10][1].duration = 200000;
	value_array[10][2].duration = 50000;

	value_array[11][0].duration = 100000;
	value_array[11][1].duration = 200000;
	value_array[11][2].duration = 50000;

	value_array[12][0].duration = 100000;
	value_array[12][1].duration = 200000;
	value_array[12][2].duration = 50000;

	value_array[13][0].duration = 100000;
	value_array[13][1].duration = 200000;
	value_array[13][2].duration = 50000;

	value_array[14][0].duration = 100000;
	value_array[14][1].duration = 200000;
	value_array[14][2].duration = 50000;

	value_array[15][0].duration = 100000;
	value_array[15][1].duration = 200000;
	value_array[15][2].duration = 50000;


	for (int i = 0; i<16; i++) {
		for (int j = 0; j<3; j++) {


			timerCalc.calculate(value_array[i][j].duration);
			value_array[i][j].t1_comparator = timerCalc.t1_comparator;
			value_array[i][j].t2_comparator = timerCalc.t2_comparator;
			value_array[i][j].t2_prescaler_pointer = timerCalc.t2_prescaler_pointer;
		}
	}



	// timerCalc.calculate(value_array[0][0].duration);
	// value_array[0][0].t1_comparator = timerCalc.t1_comparator;
	// value_array[0][0].t2_comparator = timerCalc.t2_comparator;
	// value_array[0][0].t2_prescaler_pointer = timerCalc.t2_prescaler_pointer;
	//
	// timerCalc.calculate(value_array[0][1].duration);
	// value_array[0][1].t1_comparator = timerCalc.t1_comparator;
	// value_array[0][1].t2_comparator = timerCalc.t2_comparator;
	// value_array[0][1].t2_prescaler_pointer = timerCalc.t2_prescaler_pointer;
	//
	// timerCalc.calculate(value_array[0][2].duration);
	// value_array[0][2].t1_comparator = timerCalc.t1_comparator;
	// value_array[0][2].t2_comparator = timerCalc.t2_comparator;
	// value_array[0][2].t2_prescaler_pointer = timerCalc.t2_prescaler_pointer;

	// value_array[0][0].t1_comparator = 0x0265;
	// value_array[0][0].t2_comparator = 0xFF;
	// value_array[0][0].t2_prescaler_pointer = 0x06;
	//
	// value_array[0][1].t1_comparator = 0x0157;
	// value_array[0][1].t2_comparator = 0xFF;
	// value_array[0][1].t2_prescaler_pointer = 0x04;
	//
	// value_array[0][2].t1_comparator = 0x0189;
	// value_array[0][2].t2_comparator = 0xFF;
	// value_array[0][2].t2_prescaler_pointer = 0x02;
}
