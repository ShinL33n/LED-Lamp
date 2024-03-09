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
    "Time Frame": true,      // true - normal "working" mode, false - mode for testing - no time frame set
    "Last State": true       // true - was on, false - was off
}
*/

JsonDocument DeveloperClassForTesting::CreateTestLedProfile()
{

    JsonDocument ledProfileJson;

    ledProfileJson["Colors"]["Red"] = 100;
    ledProfileJson["Colors"]["Green"] = 100;
    ledProfileJson["Colors"]["Blue"] = 20;

    ledProfileJson["Brightness"] = 80;
    ledProfileJson["White"] = 0;

    ledProfileJson["Time"]["Hours"]["Start"] = 1;
    ledProfileJson["Time"]["Hours"]["End"] = 1;
    ledProfileJson["Time"]["Minutes"]["Start"] = 20;
    ledProfileJson["Time"]["Minutes"]["End"] = 15;

    ledProfileJson["Last State"] = true;

    return ledProfileJson;
}