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
    "White hue": 6500,
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
}
*/

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


LedProfileHandler::LedProfileHandler() {
    Serial.println("[LedProfileHandler]: XD z konstruktora");
    ReadLedProfileFromFileToMemory();
}

LedProfile LedProfileHandler::GetLedProfile() {
    return _ledProfile;
}

String LedProfileHandler::GetLedProfileToJsonString() {
    return ConvertLedProfileToJsonString();
}

void LedProfileHandler::SetLedProfileFromJsonFormat(JsonDocument ledProfileJson) {//LedProfile ledProfile) {
    Serial.println("[LedProfileHandler]: XD z SetLedProfileFromJsonFormat");

    LedProfile ledProfile = ConvertJsonToLedProfile(ledProfileJson);

    _ledProfile = ledProfile;

    WriteLedProfileFromMemoryToFile();
}

void LedProfileHandler::WriteLedProfileFromMemoryToFile() {
    Serial.println("[LedProfileHandler]: XD z WriteLedProfileFromMemoryToFile");
    
    String ledProfileJsonString = ConvertLedProfileToJsonString();

    File ledProfileFile = SPIFFS.open("/LedProfile.json", "w");

    if (!ledProfileFile) {
        Serial.println("[LedProfileHandler]: Error opening file for writing.");
    }
    else {
        ledProfileFile.print(ledProfileJsonString);
    } 
 
    ledProfileFile.close();
}

void LedProfileHandler::ReadLedProfileFromFileToMemory() {
    Serial.println("[LedProfileHandler]: XD z ReadLedProfileFromFileToMemory");

    String ledProfileJsonString;

    // (TO ADD HERE): DEFAULT PROFILE FILE TO BE SURE LAMP HAS SOMETHING TO WORK WITH FROM THE BEGINING
    if(!SPIFFS.exists(("/LedProfile.json"))) {
        DeveloperClassForTesting developerClassForTesting;
        SetLedProfileFromJsonFormat(developerClassForTesting.CreateTestLedProfile());
    }

    File ledProfileFile = SPIFFS.open("/LedProfile.json", "r");
    
    if(!ledProfileFile) {
        Serial.println("[LedProfileHandler]: Error opening file for reading.");
    }
    else {
        ledProfileJsonString = ledProfileFile.readStringUntil('\n');
    }

    JsonDocument ledProfileJson;

    DeserializationError error = deserializeJson(ledProfileJson, ledProfileJsonString);
    if (error) {
        Serial.print("[LedProfileHandler]: Parsing failed:");
        Serial.println(error.f_str());
    }
    else {      
        SetLedProfileFromJsonFormat(ledProfileJson);
    }

    ledProfileFile.close();
}

String LedProfileHandler::ConvertLedProfileToJsonString() {
    JsonDocument ledProfileJson;

    ledProfileJson["Colors"]["Red"] =     _ledProfile.getRed();
    ledProfileJson["Colors"]["Green"] =   _ledProfile.getGreen();
    ledProfileJson["Colors"]["Blue"] =    _ledProfile.getBlue();

    ledProfileJson["Brightness"] = _ledProfile.getBrightness();

    ledProfileJson["Time"]["Hours"]["Start"] =    _ledProfile.getStartHour();
    ledProfileJson["Time"]["Hours"]["End"] =      _ledProfile.getEndHour();
    ledProfileJson["Time"]["Minutes"]["Start"] =  _ledProfile.getStartMinutes();
    ledProfileJson["Time"]["Minutes"]["End"] =    _ledProfile.getEndMinutes();


    String ledProfileJsonString;
    serializeJson(ledProfileJson, ledProfileJsonString);
    return ledProfileJsonString;
}

LedProfile LedProfileHandler::ConvertJsonToLedProfile(JsonDocument ledProfileJson) {
    LedProfile ledProfile;
    ledProfile.setRGB(ledProfileJson["Colors"]["Red"], ledProfileJson["Colors"]["Green"], ledProfileJson["Colors"]["Blue"]);
    ledProfile.setBrightness(ledProfileJson["Brightness"]);
    ledProfile.setWorkHours(ledProfileJson["Time"]["Hours"]["Start"],ledProfileJson["Time"]["Hours"]["End"], ledProfileJson["Time"]["Minutes"]["Start"], ledProfileJson["Time"]["Minutes"]["End"]);

    return ledProfile;
}