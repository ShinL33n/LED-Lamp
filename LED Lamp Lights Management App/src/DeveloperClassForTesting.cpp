#include "../headers/DeveloperClassForTesting.h"

JsonDocument DeveloperClassForTesting::CreateTestLedProfile()
{

    JsonDocument ledProfileJson;

    ledProfileJson["Colors"]["Red"] = 10;
    ledProfileJson["Colors"]["Green"] = 10;
    ledProfileJson["Colors"]["Blue"] = 2;

    ledProfileJson["Brightness"] = 80;

    ledProfileJson["Time"]["Hours"]["Start"] = 10;
    ledProfileJson["Time"]["Hours"]["End"] = 30;
    ledProfileJson["Time"]["Minutes"]["Start"] = 18;
    ledProfileJson["Time"]["Minutes"]["End"] = 30;

    return ledProfileJson;
}