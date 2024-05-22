#include "Storage.h"

Storage::Storage(): memory(JC_EEPROM::eeprom_size_t::kbits_2, 1, 8), magic(0x44524B4D) {}

void Storage::begin() {
    uint32_t chipMagic = 0;
    memory.begin();
    memory.read(0, reinterpret_cast<uint8_t*>(&chipMagic), 4);
    if (chipMagic != magic) {
        reset();
    }
}

void Storage::reset() {
    Config config = { 0 };
    memory.write(4, reinterpret_cast<uint8_t*>(&config), sizeof(Config));
    memory.write(0, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&magic)), 4);
}

Storage::Config Storage::readConfig() {
    Config config;
    memory.read(4, reinterpret_cast<byte*>(&config), sizeof(Config));
    return config;
}

void Storage::writeConfig(Config& config) {
    memory.write(4, reinterpret_cast<byte*>(&config), sizeof(Config));
}
