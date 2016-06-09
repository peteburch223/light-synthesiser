/*
 Name:		main.cpp
 Created:	6/9/2016 10:19:27 AM
 Author:	Pete
*/
#include <Arduino.h>
#include "TimerHelpers.h"
#include "ChannelSelector.h"
// #include "SequenceState.h"

#define VALUE_ARRAY_SIZE 16

boolean data_received;
int value_array[VALUE_ARRAY_SIZE][3];

void serial_echo_line(void);
void readback_array(int channel);
void initialize_value_array(void);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	data_received = false;
	initialize_value_array();
}

// the loop function runs over and over again until power down or reset
void loop() {

	serial_echo_line();
	if (data_received) {
		readback_array(0);
	}
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
			data_received = true;

		}
		else {
			int red = Serial.parseInt();
			int green = Serial.parseInt();
			int blue = Serial.parseInt();
			if (Serial.read() == '\n') {
				value_array[arrayPointer][0] = red;
				value_array[arrayPointer][1] = green;
				value_array[arrayPointer][2] = blue;

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
		Serial.print(value_array[i][0], HEX);
		Serial.print(value_array[i][1], HEX);
		Serial.print(value_array[i][2], HEX);
		Serial.print('\n');
	}
	Serial.println("END OF READBACK");
	data_received = false;

}

void initialize_value_array() {
	for (int i = 0; i<16; i++) {
		for (int j = 0; j<3; j++) {
			int channel = i;
			int colour = j << 4;
			value_array[i][j] = 0xFFFFFF00 | colour | channel;
		}
	}
}
