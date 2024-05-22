#include "Lock.h"

void Lock::begin() {
    servo.attach(PB15);
    servo.write(90);
}

void Lock::lock() {
    servo.write(90);
}

void Lock::unlock() {
    servo.write(180);
}
