#include <Arduino.h>
#include "Network.h"
#include "Storage.h"
#include "Lock.h"
#include "Key.h"
#include "Nfc.h"
#include "Lcd.h"

ManagementNetwork network;
Storage storage;
Lock lock;
NFC nfc;
LCD lcd;
Key key;

bool inMaintenanceMode;

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing LCD");
    lcd.begin();
    lcd.showWaiting();
    Serial.println("Initializing storage");
    storage.begin();
    Serial.println("Initializing NFC");
    nfc.begin();
    Serial.println("Initializing key");
    key.begin();
    if (key.poll()) {
        Serial.println("Entering maintenance mode");
        inMaintenanceMode = true;
        lcd.showUnderMaintenance();
        return;
    }
    Storage::Config config = storage.readConfig();
    ManagementNetwork::Configuration configuration = ManagementNetwork::Configuration(config);
    Serial.println("Initializing network");
    Serial.println(configuration);
    network.setConfiguration(configuration);
    network.begin();
    Serial.println("Initializing lock");
    lock.begin();
    lcd.showReady();
    Serial.println("Ready");
}

void loop() {
    if (inMaintenanceMode) {
        std::optional<Storage::Config> config = nfc.pollConfig();
        if (config.has_value()) {
            ManagementNetwork::Configuration configuration = ManagementNetwork::Configuration(config.value());
            Serial.println("Saving configuration");
            // Adafruit_PN532::PrintHex((byte*) &config.value(), 70);
            Serial.println(configuration);
            storage.writeConfig(config.value());
            network.setConfiguration(configuration);
            Serial.println("Initializing network");
            network.begin();
            lcd.showConfigSaved();
            delay(5000);
            lcd.showReady();
            inMaintenanceMode = false;
            Serial.println("Ready");
        }
        return;
    }
    std::optional<String> id = nfc.pollId();
    if (id.has_value()) {
        Serial.print("Accessing by ID: ");
        Serial.println(id.value());
        lcd.showAuthenticating();
        network.access(id.value());
    }
    std::optional<bool> result = network.poll();
    if (result.has_value()) {
        if (result.value()) {
            Serial.println("Access granted");
            lcd.showAllowed();
            lock.unlock();
        } else {
            Serial.println("Access denied");
            lcd.showDenied();
        }
        delay(5000);
        lcd.showReady();
        lock.lock();
        Serial.println("Ready");
    }
}
