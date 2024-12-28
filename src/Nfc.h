#pragma once
#include <Adafruit_PN532.h>
#include "Storage.h"
#include "NString.h"
#include <optional>

class NFC {
public:
    NFC();
    void begin();
    std::optional<String> pollId();
    std::optional<Storage::Config> pollConfig();

private:
    std::optional<String> readIdWithApdu();
    std::optional<Storage::Config> readConfigWithApdu();
    std::optional<String> readIdWithMifare();

private:
    uint8_t SELECT_ID_APDU[16] = {
        0x00,                                                       /* CLA */
        0xA4,                                                       /* INS */
        0x04,                                                       /* P1  */
        0x00,                                                       /* P2  */
        0x0A,                                                       /* Length of AID  */
        0xCA, 0xFE, 0x00, 0xCA, 0xFE, 0x00, 0xCA, 0xFE, 0x00, 0XFF, /* AID  */
        0x00                                                        /* Le  */
    };

    uint8_t SELECT_CONFIG_APDU[16] = {
        0x00,                                                       /* CLA */
        0xA4,                                                       /* INS */
        0x04,                                                       /* P1  */
        0x00,                                                       /* P2  */
        0x0A,                                                       /* Length of AID  */
        0xCA, 0xFE, 0x00, 0xCA, 0xFE, 0x00, 0xCA, 0xFE, 0x00, 0XFE, /* AID  */
        0x00                                                        /* Le  */
    };

    uint8_t mifareKey[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    Adafruit_PN532 nfc;
};
