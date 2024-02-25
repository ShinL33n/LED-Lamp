#include "../headers/LedManager.h"

LedManager::LedManager(Adafruit_NeoPixel *LEDStrip, LedProfileHandler *ledProfileHandler) {
    _ledStrip = LEDStrip;
    _ledProfileHandler = ledProfileHandler;

    _ledStrip->begin();
    _ledStrip->clear();

    SetColors();

    _ledStrip->show();
}

void LedManager::SetColors() {
    LedProfile ledProfile = _ledProfileHandler->GetLedProfile();

    _ledStrip->fill(_ledStrip->Color(ledProfile.getRed(), ledProfile.getGreen(), ledProfile.getBlue()));
}