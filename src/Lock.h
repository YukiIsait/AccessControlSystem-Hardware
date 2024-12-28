#pragma once
#include <Arduino.h>
#include <Servo.h>

class Lock {
public:
    void begin();
    void lock();
    void unlock();

private:
    Servo servo;
};
