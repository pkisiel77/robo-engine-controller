#pragma once

#include <Arduino.h>

extern byte* registerState;

void registerWrite(int pin, bool state);
