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
AsyncWebServer server(80);            // Create AsyncWebServer object on port 80
AsyncWebSocket webSocket("/ws");      // Create a WebSocket object

LedProfileHandler ledProfileHandler;  // Create handler for led profile to be able to manager led profile

void InitializeSPIFFS();              // Initialize file system for file storage
void InitializeWiFiConnection();      // Initialize ESP32 connection to the network (option to log into desired network via web UI)
void InitializeWebSocket();           // Initialize WebSocket for communication with client controller apps 
void InitializeLEDs();                // Initialize LEDs lighting and effects
// void InitializeSPIFFS();              // Initialize file system for file storage
// void InitializeWiFiConnection();      // Initialize ESP32 connection to the network (option to log into desired network via web UI)
// void InitializeWebSocket(AsyncWebServer *server, AsyncWebSocket *webSocket, LedProfileHandler *ledProfileHandler);           // Initialize WebSocket for communication with client controller apps 
// void InitializeLEDs(LedProfileHandler *ledProfileHandler);  


void setup() {
    Serial.begin(115200);

    InitializeSPIFFS();
    InitializeWiFiConnection();
    
// AsyncWebServer server(80);            // Create AsyncWebServer object on port 80
// AsyncWebSocket webSocket("/ws");      // Create a WebSocket object
// LedProfileHandler ledProfileHandler;  // Create handler for led profile to be able to manager led profile

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

void InitializeWebSocket() {
  WebSocketHandler webSocketHandler(&server, &webSocket, &ledProfileHandler);
  webSocketHandler.InitializeWebSocket();
}

void InitializeLEDs() {
    LedManager ledManager(&LEDStrip, &ledProfileHandler);
}

// void InitializeWebSocket(AsyncWebServer *server, AsyncWebSocket *webSocket, LedProfileHandler *ledProfileHandler) {
//   WebSocketHandler webSocketHandler(server, webSocket, ledProfileHandler);
//   webSocketHandler.InitializeWebSocket();
// }

// void InitializeLEDs(LedProfileHandler *ledProfileHandler) {
//     LedManager ledManager(&LEDStrip, ledProfileHandler);
// }
