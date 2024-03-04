#pragma once

#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "LedProfile.cpp"
#include "DeveloperClassForTesting.h"

class LedProfileHandler
{
private:
    LedProfile *_ledProfile;

    void SaveLedProfileToFile(JsonDocument ledProfileJson);
    void ReadLedProfileFromFile();

    LedProfile ConvertJsonToLedProfile(JsonDocument ledProfileJson);
    String ConvertLedProfileToJsonString(LedProfile ledProfile);

public:
    LedProfileHandler(LedProfile *ledProfile);
    void SetLedProfileFromJsonFormat(JsonDocument ledProfileJson);
    String GetLedProfileToJsonString();
    LedProfile GetLedProfile();

};