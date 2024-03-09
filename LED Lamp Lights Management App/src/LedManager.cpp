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

void LedManager::On()
{
    // ApplyLastState();
    ApplyColors();
    ApplyBrightness();
    ApplyWhiteHue();
}

void LedManager::Off()
{
    _ledStrip->clear();
    _ledStrip->show();
}

void LedManager::ApplyColors()
{
    _ledStrip->fill(_ledStrip->Color(_ledProfile.getRed(), _ledProfile.getGreen(), _ledProfile.getBlue()));
    _ledStrip->show();
}

void LedManager::ApplyBrightness()
{
    _ledStrip->setBrightness(_ledProfile.getBrightness());
    _ledStrip->show();
}

void LedManager::ApplyWhiteHue()
{
    _ledStrip->fill(_ledStrip->Color(_ledProfile.getRed(), _ledProfile.getGreen(), _ledProfile.getBlue(), _ledProfile.getWhite()));
    _ledStrip->show();
}

void LedManager::ApplyLastState()
{
    if (_ledProfile.getLastState() || !_ledProfile.getTimeFrame())
        On();
    else
        Off();
}