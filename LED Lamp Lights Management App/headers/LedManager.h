#pragma once

#include <Adafruit_NeoPixel.h>
#include "LedProfileHandler.h"

class LedManager
{
private:
    Adafruit_NeoPixel *_ledStrip;
    LedProfile _ledProfile;

    void InitializeStrip();
    void ApplyColors();
    void ApplyBrightness();
    void ApplyWhiteHue();

public:
    LedManager(Adafruit_NeoPixel *LEDStrip, LedProfile *ledProfile);
    void ApplyLastState();
    void On();
    void Off();
};