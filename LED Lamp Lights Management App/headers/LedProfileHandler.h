#pragma once

#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "LedProfile.cpp"
#include "DeveloperClassForTesting.h"

class LedProfileHandler
{
private:
    LedProfile _ledProfile;

    void WriteLedProfileFromMemoryToFile();
    void ReadLedProfileFromFileToMemory();

    LedProfile ConvertJsonToLedProfile(JsonDocument ledProfileJson);
    String ConvertLedProfileToJsonString();

public:
    LedProfileHandler();
    void SetLedProfileFromJsonFormat(JsonDocument ledProfileJson);
    String GetLedProfileToJsonString();
    LedProfile GetLedProfile();

};