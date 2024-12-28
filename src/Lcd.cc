#include "Lcd.h"

LCD::LCD(): lcd(0x3F, 16, 2) {}

void LCD::begin() {
    lcd.init();
    lcd.backlight();
}

void LCD::showReady() {
    lcd.clear();
    lcd.print("Tap your card");
    lcd.setCursor(7, 1);
    lcd.print("to unlock");
}

void LCD::showAllowed() {
    lcd.clear();
    lcd.print("Access granted");
}

void LCD::showDenied() {
    lcd.clear();
    lcd.print("Access denied");
}

void LCD::showWaiting() {
    lcd.clear();
    lcd.print("Waiting...");
}

void LCD::showAuthenticating() {
    lcd.clear();
    lcd.print("Authenticating");
    lcd.setCursor(13, 1);
    lcd.print("...");
}

void LCD::showUnderMaintenance() {
    lcd.clear();
    lcd.print("Under");
    lcd.setCursor(5, 1);
    lcd.print("maintenance");
}

void LCD::showConfigSaved() {
    lcd.clear();
    lcd.print("Config saved");
}
