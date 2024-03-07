#pragma once

#include <Adafruit_NeoPixel.h>
#include "LedProfileHandler.h"

class LedManager
{
private:
    Adafruit_NeoPixel *_ledStrip;
    LedProfileHandler *_ledProfileHandler;
    LedProfile _ledProfile;

    void InitializeStrip();
    void ApplyColors();
    void ApplyBrightness();
    void ApplyWhiteHue();
    // void ApplyLastState();

public:
    LedManager(Adafruit_NeoPixel *LEDStrip, LedProfile *ledProfile);
    void On();
    void Off();
};