#pragma once
#include <Arduino.h>
#include <JC_EEPROM.h>
#include <stdint.h>

class Storage {
public:
    struct Config {
        uint8_t mac[6];
        uint8_t ip[4];
        uint8_t subnet[4];
        uint8_t gateway[4];
        uint8_t dns[4];
        uint8_t server[4];
        uint16_t port;
        uint8_t idSize;
        uint8_t id[41];
    };

public:
    Storage();
    void begin();
    void reset();
    Config readConfig();
    void writeConfig(Config& config);

private:
    JC_EEPROM memory;
    const uint32_t magic;
};
