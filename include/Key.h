#pragma once
#include <Arduino.h>

class Key {
public:
    void begin();
    bool poll();
};
