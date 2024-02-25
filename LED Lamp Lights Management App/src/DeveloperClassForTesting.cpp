#include "../headers/DeveloperClassForTesting.h"

JsonDocument DeveloperClassForTesting::CreateTestLedProfile() {
    
    Serial.println("[DeveloperClassForTesting]: XD z CreateTestLedProfile");

    JsonDocument ledProfileJson;

    ledProfileJson["Colors"]["Red"] = 255;
    ledProfileJson["Colors"]["Green"] = 255;
    ledProfileJson["Colors"]["Blue"] = 30;

    ledProfileJson["Brightness"] = 80;

    ledProfileJson["Time"]["Hours"]["Start"] = 10;
    ledProfileJson["Time"]["Hours"]["End"] = 30;
    ledProfileJson["Time"]["Minutes"]["Start"] = 18;
    ledProfileJson["Time"]["Minutes"]["End"] = 30;

    return ledProfileJson;
}