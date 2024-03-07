#include "../headers/DeveloperClassForTesting.h"

/*
JSON example:
{
    "Type": "setLedProfile",
    "Colors": {
        "Red": 255,
        "Green": 255,
        "Blue": 40
    },
    "Brightness": 80,
    "White": 255,
    "Time": {
        "Hours": {
            "Start": 10,
            "End": 18
        },
        "Minutes": {
            "Start": 15,
            "End": 15
        }
    },
    "Apply Time": true,      // true - normal "working" mode, false - mode for testing - no time frame set
    "Last State": true       // true - was on, false - was off
}
*/

JsonDocument DeveloperClassForTesting::CreateTestLedProfile()
{

    JsonDocument ledProfileJson;

    ledProfileJson["Colors"]["Red"] = 10;
    ledProfileJson["Colors"]["Green"] = 10;
    ledProfileJson["Colors"]["Blue"] = 2;

    ledProfileJson["Brightness"] = 80;
    ledProfileJson["White"] = 0;

    ledProfileJson["Time"]["Hours"]["Start"] = 10;
    ledProfileJson["Time"]["Hours"]["End"] = 30;
    ledProfileJson["Time"]["Minutes"]["Start"] = 18;
    ledProfileJson["Time"]["Minutes"]["End"] = 00;

    // ledProfileJson["Last State"] = true;

    return ledProfileJson;
}