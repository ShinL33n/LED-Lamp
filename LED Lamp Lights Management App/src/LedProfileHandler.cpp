#include "../headers/LedProfileHandler.h"

/*
jak moze wygladać json:
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
    }
    "Last State": true; //
}
*/

/*
// odczytujemy profil z pliku i ładujemy do do instancji LedProfile:
// -na początku programu
// -kiedy profil zostanie nadpisany czyli kiedy otrzyma "setLedProfile" w WebSocketHandler
// można to zrobić jako metoda pubiczna w tej klasie np ReadLedProfileFromFileToMemory()
// scenariusz użycia:
// -pogram się rozpoczyna, czytamy z pliku LedProfile.json dane do instancji klasy LedProfile
// -przychodzi wiadomość "setLedProfile" od klienta, więc NADPISUJEMY instancje klasy LedProfile i dodatkowo zapisujemy profil do pliku (żeby w razie resetu lampy załadował się poprawnie ostatio ustawiony)


// void LedProfileHandler::SetLedProfile(LedProfile ledProfile) {
//     _ledProfile.setRGB(ledProfile.getRed(), ledProfile.getGreen(), ledProfile.getBlue());
//     _ledProfile.setBrightness(ledProfile.getBrightness());
//     _ledProfile.setWorkHours(ledProfile.getStartHour(), ledProfile.getEndHour(), ledProfile.getStartMinutes(), ledProfile.getEndMinutes());
// }
*/

template <>
JsonDocument LedProfileHandler::ConvertLedProfileTo(LedProfile ledProfile)
{
    return AssignLedProfileJson(ledProfile);
}

template <>
String LedProfileHandler::ConvertLedProfileTo(LedProfile ledProfile)
{
    String ledProfileJsonString;
    serializeJson(AssignLedProfileJson(ledProfile), ledProfileJsonString);
    return ledProfileJsonString;
}

LedProfileHandler::LedProfileHandler(LedProfile *ledProfile)
{
    _ledProfile = ledProfile;
}

LedProfile LedProfileHandler::GetLedProfile()
{
    return *_ledProfile;
}

String LedProfileHandler::GetLedProfileToJsonString()
{
    return ConvertLedProfileTo<String>(*_ledProfile);
}

void LedProfileHandler::ReadLedProfileFromFile()
{

    // (TO ADD HERE): DEFAULT PROFILE FILE TO BE SURE LAMP HAS SOMETHING TO WORK WITH FROM THE BEGINING
    if (!SPIFFS.exists(("/LedProfile.json")))
    {
        DeveloperClassForTesting developerClassForTesting;
        JsonDocument mockLedProfileJson;
        mockLedProfileJson = developerClassForTesting.CreateTestLedProfile();
        SetLedProfileFromJsonFormat(mockLedProfileJson);
    }

    String ledProfileJsonString;
    File ledProfileFile = SPIFFS.open("/LedProfile.json", "r");

    if (!ledProfileFile)
    {
        Serial.println("[LedProfileHandler]: An error occured trying to open a led profile file for reading.");
    }
    else
    {
        ledProfileJsonString = ledProfileFile.readStringUntil('\n');
    }
    ledProfileFile.close();

    JsonDocument ledProfileJson;
    DeserializationError deserializationError = deserializeJson(ledProfileJson, ledProfileJsonString);

    if (deserializationError)
    {
        Serial.printf("[LedProfileHandler]: Parse operation has failed trying to read led profile from a file (%s). \n", deserializationError.f_str());
    }
    else
    {
        *_ledProfile = ConvertJsonToLedProfile(ledProfileJson);

        Serial.print("[LedProfileHandler]: Led profile has been loaded from the configuration file and been saved to the memory with the following data: ");
        Serial.print(ledProfileJsonString);
        Serial.println();
    }
}

void LedProfileHandler::SetLedProfile(LedProfile ledProfile)
{
    JsonDocument ledProfileJson;

    ledProfileJson = ConvertLedProfileTo<JsonDocument>(ledProfile);

    SaveLedProfileToFile(ledProfileJson);
}

void LedProfileHandler::SetLedProfileFromJsonFormat(JsonDocument ledProfileJson)
{
    LedProfile ledProfile;

    if (!ledProfileJson.containsKey("Last State"))
        ledProfileJson["Last State"] = (*_ledProfile).getLastState();

    ledProfile = ConvertJsonToLedProfile(ledProfileJson);

    *_ledProfile = ledProfile;

    SaveLedProfileToFile(ledProfileJson);
}

void LedProfileHandler::SaveLedProfileToFile(JsonDocument ledProfileJson)
{
    String ledProfileJsonString;
    serializeJson(ledProfileJson, ledProfileJsonString);

    File ledProfileFile = SPIFFS.open("/LedProfile.json", "w");

    if (!ledProfileFile)
    {
        Serial.println("[LedProfileHandler]: Error opening file for writing.");
    }
    else
    {
        if (ledProfileFile.println(ledProfileJsonString))
        {
            Serial.print("[LedProfileHandler]: New led profile has been saved with the following content: ");
            Serial.print(ledProfileJsonString);
            Serial.println();
        }
        else
            Serial.println("[LedProfileHandler]: New led profile has failed to be saved.");
    }

    ledProfileFile.close();
}

JsonDocument LedProfileHandler::AssignLedProfileJson(LedProfile ledProfile)
{
    JsonDocument ledProfileJson;
    ledProfileJson["Colors"]["Red"] = ledProfile.getRed();
    ledProfileJson["Colors"]["Green"] = ledProfile.getGreen();
    ledProfileJson["Colors"]["Blue"] = ledProfile.getBlue();
    ledProfileJson["Brightness"] = ledProfile.getBrightness();
    ledProfileJson["White"] = ledProfile.getWhite();
    ledProfileJson["Time"]["Hours"]["Start"] = ledProfile.getStartHour();
    ledProfileJson["Time"]["Hours"]["End"] = ledProfile.getEndHour();
    ledProfileJson["Time"]["Minutes"]["Start"] = ledProfile.getStartMinutes();
    ledProfileJson["Time"]["Minutes"]["End"] = ledProfile.getEndMinutes();
    ledProfileJson["Last State"] = ledProfile.getLastState();

    return ledProfileJson;
}

// template <typename T>
// T ConvertLedProfileTo(LedProfile ledProfile);

/*template <typename T>
T ConvertLedProfileTo(LedProfile ledProfile)
{
    JsonDocument ledProfileJson;
    ledProfileJson["Colors"]["Red"] = ledProfile.getRed();
    ledProfileJson["Colors"]["Green"] = ledProfile.getGreen();
    ledProfileJson["Colors"]["Blue"] = ledProfile.getBlue();
    ledProfileJson["Brightness"] = ledProfile.getBrightness();
    ledProfileJson["White"] = ledProfile.getWhite();
    ledProfileJson["Time"]["Hours"]["Start"] = ledProfile.getStartHour();
    ledProfileJson["Time"]["Hours"]["End"] = ledProfile.getEndHour();
    ledProfileJson["Time"]["Minutes"]["Start"] = ledProfile.getStartMinutes();
    ledProfileJson["Time"]["Minutes"]["End"] = ledProfile.getEndMinutes();
    ledProfileJson["Last State"] = ledProfile.getLastState();

    // Gdy typ T jest JsonDocument
    if constexpr (std::is_same_v<T, JsonDocument>)
    {

        return ledProfileJson;
    }
    // Gdy typ T jest String
    else if constexpr (std::is_same_v<T, String>)
    {
        String ledProfileJsonString;
        serializeJson(ledProfileJson, ledProfileJsonString);

        return ledProfileJsonString;
    }
    // Rzucenie wyjątku dla nieobsługiwanego typu T
    else
    {
        throw std::runtime_error("[Runtime Error]: Unsupported generic specified.");
    }
}*/

// template <typename T>
// T LedProfileHandler::ConvertLedProfileTo(LedProfile ledProfile)
// {
//     JsonDocument ledProfileJson;

//     // Set Colors
//     ledProfileJson["Colors"]["Red"] = ledProfile.getRed();
//     ledProfileJson["Colors"]["Green"] = ledProfile.getGreen();
//     ledProfileJson["Colors"]["Blue"] = ledProfile.getBlue();

//     // Set Brightness
//     ledProfileJson["Brightness"] = ledProfile.getBrightness();

//     // Set White
//     ledProfileJson["White"] = ledProfile.getWhite();

//     // Set Time
//     ledProfileJson["Time"]["Hours"]["Start"] = ledProfile.getStartHour();
//     ledProfileJson["Time"]["Hours"]["End"] = ledProfile.getEndHour();
//     ledProfileJson["Time"]["Minutes"]["Start"] = ledProfile.getStartMinutes();
//     ledProfileJson["Time"]["Minutes"]["End"] = ledProfile.getEndMinutes();

//     // Set Last State
//     ledProfileJson["Last State"] = ledProfile.getLastState();

//     if (std::is_same<T, JsonDocument>::value)
//         return ledProfileJson;

//     String ledProfileJsonString;
//     serializeJson(ledProfileJson, ledProfileJsonString);
//     if (std::is_same<T, String>::value)
//         return ledProfileJsonString;
//     else
//         throw std::runtime_error("[Runtime Error]: Unsupported generic specified.");
// }

LedProfile LedProfileHandler::ConvertJsonToLedProfile(JsonDocument ledProfileJson)
{
    LedProfile ledProfile;

    ledProfile.setRGB(ledProfileJson["Colors"]["Red"], ledProfileJson["Colors"]["Green"], ledProfileJson["Colors"]["Blue"]);
    ledProfile.setBrightness(ledProfileJson["Brightness"]);
    ledProfile.setWhite(ledProfileJson["White"]);
    ledProfile.setWorkHours(ledProfileJson["Time"]["Hours"]["Start"], ledProfileJson["Time"]["Hours"]["End"], ledProfileJson["Time"]["Minutes"]["Start"], ledProfileJson["Time"]["Minutes"]["End"]);
    ledProfile.setLastState(ledProfileJson["Last State"]);

    // // We want to set 'last state' into memory profile while reading from file,
    // // but doesn't want it while saving incoming WebSocket message so:
    // if (ledProfileJson.containsKey("Last State"))
    //     // profile read from file contains last state key so we want to set it
    //     ledProfile.setLastState(ledProfileJson["Last State"]);
    // else
    // // profile that came from WebSocket must not contain last state key,
    // // because it is not a type we set by hand (it changes automatically
    // // by lamp time frames when LEDs turn on/off)
    // {
    //     Serial.println((*_ledProfile).getLastState());
    //     ledProfile.setLastState((*_ledProfile).getLastState());
    // }
    // // POTENTIAL ERROR HERE -----------^

    return ledProfile;
}