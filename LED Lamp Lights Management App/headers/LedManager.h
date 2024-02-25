#pragma once

#include <Adafruit_NeoPixel.h>
#include "LedProfileHandler.h"

class LedManager
{
private:
    Adafruit_NeoPixel _ledStrip;
    LedProfileHandler _ledProfileHandler;

    void SetColors();

public:
    LedManager(Adafruit_NeoPixel LEDStrip, LedProfileHandler *ledProfileHandler);

};