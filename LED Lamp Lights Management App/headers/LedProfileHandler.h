#pragma once

#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "LedProfile.cpp"
#include "DeveloperClassForTesting.h"

class LedProfileHandler
{
private:
    LedProfile *_ledProfile;

    template <typename T>
    T ConvertLedProfileTo(LedProfile ledProfile);
    // String ConvertLedProfileToJsonString(LedProfile ledProfile);

    void SaveLedProfileToFile(JsonDocument ledProfileJson);

    JsonDocument AssignLedProfileJson(LedProfile ledProfile);
    LedProfile ConvertJsonToLedProfile(JsonDocument ledProfileJson);

public:
    LedProfileHandler(LedProfile *ledProfile);
    void SetLedProfile(LedProfile ledProfile);
    void SetLedProfileFromJsonFormat(JsonDocument ledProfileJson);
    void ReadLedProfileFromFile();
    String GetLedProfileToJsonString();
    LedProfile GetLedProfile();
};