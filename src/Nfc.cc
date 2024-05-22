#include "Nfc.h"

NFC::NFC(): nfc(PA1, &Serial2) {}

void NFC::begin() {
    nfc.begin();
}

std::optional<String> NFC::pollId() {
    std::optional<String> result = std::nullopt;
    if (nfc.inListPassiveTarget()) {
        result = readIdWithApdu();
        if (result == std::nullopt) {
            result = readIdWithMifare();
        }
    }
    nfc.begin();
    return result;
}

std::optional<Storage::Config> NFC::pollConfig() {
    std::optional<Storage::Config> result = std::nullopt;
    if (nfc.inListPassiveTarget()) {
        result = readConfigWithApdu();
    }
    nfc.begin();
    return result;
}

std::optional<String> NFC::readIdWithApdu() {
    uint8_t responseBuffer[64];
    uint8_t responseLength = 64;
    if (nfc.inDataExchange(SELECT_ID_APDU, sizeof(SELECT_ID_APDU), responseBuffer, &responseLength) &&
        responseBuffer[0] > 0 &&
        responseBuffer[0] <= 40) {
        return NString(reinterpret_cast<const char*>(responseBuffer + 1), responseBuffer[0]);
    }
    return std::nullopt;
}

std::optional<Storage::Config> NFC::readConfigWithApdu() {
    Storage::Config config;
    uint8_t responseLength = 64;
    if (nfc.inDataExchange(SELECT_CONFIG_APDU,
                           sizeof(SELECT_CONFIG_APDU),
                           reinterpret_cast<uint8_t*>(&config),
                           &responseLength)) {
        return config;
    }
    return std::nullopt;
}

std::optional<String> NFC::readIdWithMifare() {
    uint8_t data[48];
    uint8_t uid[7];
    uint8_t uidLength = 7;

    if (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength) ||
        uidLength != 4) {
        return std::nullopt;
    }
    if (!nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, mifareKey)) {
        return std::nullopt;
    }
    for (uint8_t i = 0; i < 3; i++) {
        if (!nfc.mifareclassic_ReadDataBlock(i + 4, data + (i << 4))) {
            return std::nullopt;
        }
    }
    if (data[0] > 0 && data[0] <= 40) {
        return NString(reinterpret_cast<const char*>(data + 1), data[0]);
    }
    return std::nullopt;
}
