#include "register.h"

void registerWrite(int pin, bool state) {
	int reg = pin / 8;
	int actualPin = pin - (8 * reg);

	digitalWrite(7, LOW);

	for (int i = 0; i < 2; i++){
		byte* states = &registerState[i];

		if (i == reg){
			bitWrite(*states, actualPin, state);
		}

		shiftOut(4, 8, MSBFIRST, *states);
	}

	digitalWrite(7, HIGH);
}
