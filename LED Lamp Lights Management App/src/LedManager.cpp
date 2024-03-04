#include "../headers/LedManager.h"

LedManager::LedManager(Adafruit_NeoPixel *LEDStrip, LedProfile *ledProfile)
{
    _ledStrip = LEDStrip;
    _ledProfile = *ledProfile;

    InitializeStrip();
}

void LedManager::InitializeStrip()
{
    _ledStrip->begin();
    _ledStrip->clear();
}

void LedManager::ApplyColors()
{
    _ledStrip->fill(_ledStrip->Color(_ledProfile.getRed(), _ledProfile.getGreen(), _ledProfile.getBlue()));
    _ledStrip->show();
}

void LedManager::ApplyBrightness()
{
}

void LedManager::ApplyWhiteHue()
{
}

void LedManager::ApplyTime()
{
}