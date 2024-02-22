#include <Arduino.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "../headers/WebSocketHandler.h"

#define PIN         4
#define NUMPIXELS   8


Adafruit_NeoPixel LED_Strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // Initialize NeoPixel LED Strip
AsyncWebServer *server = new AsyncWebServer(80);                    // Create AsyncWebServer object on port 80
AsyncWebSocket *webSocket = new AsyncWebSocket("/ws");              // Create a WebSocket object


void InitializeWiFiConnection();    // Initialize ESP32 connection to the network (option to log into desired network via web UI)
void InitializeSPIFFS();            // Initialize file system for file storage
void InitializeWebSocket();         // Initialize WebSocket for communication with client controller apps 


void setup() {
    Serial.begin(115200);

    InitializeWiFiConnection();
    InitializeSPIFFS();
    InitializeWebSocket();
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
    WebSocketHandler webSocketHandler(server, webSocket); // For handling WebSocket events and received messages
    webSocketHandler.InitializeWebSocket();
}