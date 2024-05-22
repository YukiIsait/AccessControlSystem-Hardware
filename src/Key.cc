#include "Key.h"

void Key::begin() {
    pinMode(PB4, OUTPUT);
    digitalWrite(PB4, LOW);
    pinMode(PB3, INPUT_PULLUP);
}

bool Key::poll() {
    return digitalRead(PB3) == LOW;
}
