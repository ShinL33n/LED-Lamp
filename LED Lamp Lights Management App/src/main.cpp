#include <Arduino.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "../headers/LedProfileHandler.h"
#include "../headers/LedManager.h"

#define PIN         4
#define NUMPIXELS   8

Adafruit_NeoPixel LEDStrip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // Initialize NeoPixel LED Strip
LedProfile ledProfile;
LedProfileHandler ledProfileHandler(&ledProfile);                  // Create handler for led profile to be able to manager led profile
AsyncWebServer server(80);                                        // Create AsyncWebServer object on port 80
AsyncWebSocket webSocket("/ws");                                  // Create a WebSocket object



void InitializeSPIFFS();              // Initialize file system for file storage
void InitializeWiFiConnection();      // Initialize ESP32 connection to the network (option to log into desired network via web UI)
void InitializeLEDs(Adafruit_NeoPixel *LEDStrip, LedProfileHandler *ledProfileHandler);                // Initialize LEDs lighting and effects

void InitializeWebSocket();           // Initialize WebSocket for communication with client controller apps
void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);    // Handles different WebSocket events
void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);                                   // Handles messages received via WebSocket
void HandleReceivedMessage(JsonDocument receivedDataJsonDocument, AsyncWebSocketClient *client);




void setup() {
    Serial.begin(115200);

    InitializeSPIFFS();
    InitializeWiFiConnection();
    InitializeWebSocket();

    InitializeLEDs(&LEDStrip, &ledProfileHandler);
}

void loop() {
  
}

void InitializeWiFiConnection() {
    WiFi.mode(WIFI_STA);

    WiFiManager wifiManager;

    bool wifiResult = wifiManager.autoConnect("LED Lamp WiFi connection configuration", "ledpassword");

    if(!wifiResult) {
        Serial.printf("[WiFiManager]: Failed to connect to %s. \n", wifiManager.getWiFiSSID());
    } 
    else {
        Serial.printf("[WiFiManager]: Connected to desired WiFi (SSID: %s). \n", wifiManager.getWiFiSSID());
    }
}

void InitializeSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS]: An error has occurred while mounting SPIFFS.");
  }
  Serial.println("[SPIFFS]: SPIFFS mounted successfully.");
}

void InitializeWebSocket() {
  webSocket.onEvent(OnEvent);
  server.addHandler(&webSocket);


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.begin();
}


void InitializeLEDs(Adafruit_NeoPixel *LEDStrip, LedProfileHandler *ledProfileHandler) {
  LedManager ledManager(LEDStrip, ledProfileHandler);
}




void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[WebSocketHandler]: WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("[WebSocketHandler]: WebSocket client #%u disconnected \n", client->id());
      break;

    case WS_EVT_DATA:
      Serial.printf("WebSocketHandler]: WebSocket client #%u sent: %s \n", client->id(), (char*)data);
      HandleWebSocketData(arg, data, len, client);
      break;

    case WS_EVT_ERROR:
      Serial.printf("[WebSocketHandler]: WebSocket event error occured.");
      break;
  }
}

void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

	  JsonDocument receivedJsonDocument;
    
    // Parse received data into JSON document
    DeserializationError deserializationError = deserializeJson(receivedJsonDocument, (char*)data);    

    if (deserializationError) {
      Serial.printf("[WebSocketHandler]: Parse operation has failed trying to read led profile from incoming data (%s). \n", deserializationError.f_str());
    }
    else {
      HandleReceivedMessage(receivedJsonDocument, client);
    }
  }
}

// zaimplementować JSON'a bo wiadomości będę w jego formie (wartość type np doc["type"] == "getLedProfile" albo doc["type"] == "setLedProfile")
void HandleReceivedMessage(JsonDocument receivedJsonDocument, AsyncWebSocketClient *client)
{
  if(receivedJsonDocument["Type"] == "getLedProfile") {
    // DEBBUG LINE | DEBBUG LINE | DEBBUG LINE | DEBBUG LINE | DEBBUG LINE
    Serial.println("[DEBBUG | WebSocketHandler]: INSIDE \"getLedProfile\" IN HandleReceivedMessage");    
    webSocket.text(client->id(), ledProfileHandler.GetLedProfileToJsonString());
  }

  else if(receivedJsonDocument["Type"] == "setLedProfile") {
    // DEBBUG LINE | DEBBUG LINE | DEBBUG LINE | DEBBUG LINE | DEBBUG LINE
    Serial.println("[DEBBUG | WebSocketHandler]: INSIDE \"setLedProfile\" IN HandleReceivedMessage");
    ledProfileHandler.SetLedProfileFromJsonFormat(receivedJsonDocument);
  }

  else {
    Serial.print("[WebSocketHandler]: Unsupported message received.");
  }
}







// void InitializeLEDs(Adafruit_NeoPixel *LEDStrip, LedProfileHandler *ledProfileHandler) {
//   LedManager ledManager(LEDStrip, ledProfileHandler);
// }

// void InitializeWebSocket(AsyncWebServer *server, AsyncWebSocket *webSocket, LedProfileHandler *ledProfileHandler) {
//   WebSocketHandler webSocketHandler(server, webSocket, ledProfileHandler);
//   webSocketHandler.InitializeWebSocket();
// }

// void InitializeLEDs(LedProfileHandler *ledProfileHandler) {
//     LedManager ledManager(&LEDStrip, ledProfileHandler);
// }
