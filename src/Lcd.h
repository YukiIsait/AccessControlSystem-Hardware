#pragma once
#include <LiquidCrystal_I2C.h>

class LCD {
public:
    LCD();
    void begin();
    void showReady();
    void showAllowed();
    void showDenied();
    void showWaiting();
    void showAuthenticating();
    void showUnderMaintenance();
    void showConfigSaved();

private:
    LiquidCrystal_I2C lcd;
};
