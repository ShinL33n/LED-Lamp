#include <Arduino.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "../headers/WebSocketHandler.h"
#include "../headers/LedProfileHandler.h"
#include "../headers/LedManager.h"

#define PIN         4
#define NUMPIXELS   8


Adafruit_NeoPixel LEDStrip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // Initialize NeoPixel LED Strip
// AsyncWebServer *server = new AsyncWebServer(80);                    // Create AsyncWebServer object on port 80
// AsyncWebSocket *webSocket = new AsyncWebSocket("/ws");              // Create a WebSocket object
AsyncWebServer server(80);                    // Create AsyncWebServer object on port 80
AsyncWebSocket webSocket("/ws");              // Create a WebSocket object

LedProfileHandler ledProfileHandler;
//LedProfileHandler ledProfileHandler;

void InitializeWiFiConnection();    // Initialize ESP32 connection to the network (option to log into desired network via web UI)
void InitializeSPIFFS();            // Initialize file system for file storage
void InitializeWebSocket();         // Initialize WebSocket for communication with client controller apps 
void InitializeLEDs();              // Initialize LEDs lighting and effects

void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);    // Handles different WebSocket events
void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);                                   // Handles messages received via WebSocket
void HandleReceivedMessage(JsonDocument receivedDataJsonDocument, AsyncWebSocketClient *client);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void notifyClients(String ledProfile) {
//   webSocket.textAll(ledProfile);
// }

// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
//   AwsFrameInfo *info = (AwsFrameInfo*)arg;
//   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
//     data[len] = 0;
//     //Check if the message is "getLedProfile"
//     if (strcmp((char*)data, "getLedProfile") == 0) {
      
//       notifyClients("loooo");
//     }
//     else {

//     }
//   }
// }

// void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
//   switch (type) {
//     case WS_EVT_CONNECT:
//       Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
//       break;
//     case WS_EVT_DISCONNECT:
//       Serial.printf("WebSocket client #%u disconnected\n", client->id());
//       break;
//     case WS_EVT_DATA:
//     handleWebSocketMessage(arg, data, len);
//       break;
//     case WS_EVT_PONG:
//     case WS_EVT_ERROR:
//       break;
//   }
// }

// void InitializeWebSocket() {
//   webSocket.onEvent(onEvent);
//   server.addHandler(&webSocket);
// }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
    Serial.begin(115200);

    InitializeWiFiConnection();
    InitializeSPIFFS();
    InitializeWebSocket();
    InitializeLEDs();
}

void loop() {
  
}

void InitializeWiFiConnection() {
    WiFi.mode(WIFI_STA);

    WiFiManager wifiManager;

    bool wifiResult;
    wifiResult = wifiManager.autoConnect("LED Lamp WiFi connection configuration","ledpassword");

    if(!wifiResult) {
        Serial.println("Failed to connect.");
        // ESP.restart();
    } 
    else {
        Serial.println("Connected to desired WiFi.");
    }
}

void InitializeSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// void InitializeWebSocket() {
//   WebSocketHandler webSocketHandler(&server, &webSocket, ledProfileHandler);
//   webSocketHandler.InitializeWebSocket();
// }

void InitializeLEDs() {
    //LedManager ledManager(LEDStrip, &ledProfileHandler);

    LEDStrip.begin();
    LEDStrip.clear();

    LedProfile ledProfile = ledProfileHandler.GetLedProfile();

    LEDStrip.fill(LEDStrip.Color(ledProfile.getRed(), ledProfile.getGreen(), ledProfile.getBlue()));

    LEDStrip.show();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void InitializeWebSocket() {
    webSocket.onEvent(OnEvent);
    server.addHandler(&webSocket);
Serial.println("[Main]: XD na końcu InitializeWebSocket");

}

void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    
Serial.println("[Main]: XD na początku OnEvent");
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[WebSocketHandler]: WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("[WebSocketHandler]: WebSocket client #%u disconnected\n", client->id());
      break;

    case WS_EVT_DATA:
      HandleWebSocketData(arg, data, len, client);
      break;

    case WS_EVT_ERROR:
      Serial.println("[WebSocketHandler]: WebSocket event error occured.");
      break;
  }
Serial.println("[Main]: XD na końcu OnEvent");
}

void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
Serial.println("[Main]: XD na początku HandleWebSocketData");
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

	  JsonDocument receivedJsonDocument;
    
    DeserializationError error = deserializeJson(receivedJsonDocument, (char*)data);

    if (error) {
      Serial.print("[WebSocketHandler]: Parsing failed:");
      Serial.println(error.f_str());
    }
    else {
      HandleReceivedMessage(receivedJsonDocument, client);
    }
  }
Serial.println("[Main]: XD na końcu HandleWebSocketData");

}

// zaimplementować JSON'a bo wiadomości będę w jego formie (wartość type np doc["type"] == "getLedProfile" albo doc["type"] == "setLedProfile")
void HandleReceivedMessage(JsonDocument receivedJsonDocument, AsyncWebSocketClient *client) {  
Serial.println("[Main]: XD na początku HandleReceivedMessage");

  if(strcmp(receivedJsonDocument["Type"], "getLedProfile") == 0) {
    // musimy uzyskać json format profilu, bo tak się posługujemu w programie dla wygody, przekonwertować go na stringa i podać w wiadomość
    
    webSocket.text((uint32_t)client, ledProfileHandler.GetLedProfileToJsonString()); /////////////////////////////////////////////
  }

  else if(strcmp(receivedJsonDocument["Type"], "setLedProfile") == 0) {
    // tutaj odwrotnie, musimy przekonwertować stringa w jsona i wysłać do zapisania do pliku
    // przydała by się też wiadomość że plik zostać zapisany i można go odczytać z pliku jako nowy profil żeby nie robić tego za każdym razem kiedy potrzebny jest profil
   
    ledProfileHandler.SetLedProfileFromJsonFormat(receivedJsonDocument);
    // MAMY JSON'a / JSON Stringa
    // musimy móc zapisać profil używając JSONA
    // muimy móc odczytać profil w formie stringa używając JSONA 
  }

  else {
    Serial.print("[WebSocketHandler]: Unsupported message.");
  }
Serial.println("[Main]: XD na końcu HandleReceivedMessage");

}