#include <Arduino.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "../headers/WebSocketHandler.h"

#define PIN         4
#define NUMPIXELS   8


Adafruit_NeoPixel LED_Strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // Initialize NeoPixel LED Strip
AsyncWebServer server(80);                                          // Create AsyncWebServer object on port 80
AsyncWebSocket webSocket("/ws");                                    // Create a WebSocket object



void InitializeWiFiConnection();    // Initialize ESP32 connection to the network (option to log into desired network via web UI)
void InitializeSPIFFS();            // Initialize file system for file storage
void InitializeWebSocket();         // Initialize WebSocket for communication with client controller apps 
void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);    // Handles different WebSocket events
void HandleWebSocketMessages(void *arg, uint8_t *data, size_t len); // Handles messages received via WebSocket


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
    // webSocket.onEvent(OnEvent);
    // server.addHandler(&webSocket);
    WebSocketHandler webSocketHandler(&server, &webSocket); // For handling WebSocket events and received messages
}

/* void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;

    case WS_EVT_DATA:
      HandleWebSocketMessages(arg, data, len);
      break;

    case WS_EVT_PONG:
        break;
    case WS_EVT_ERROR:
        Serial.printf("WebSocket event error occured.");
      break;
  }
}

void HandleWebSocketMessages(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String receivedMessage = (char*)data;

    // Check if the message is "getLedProfile" to send present LED profile to the client
    if (strcmp((char*)data, "getLedProfile") == 0) {
      //String ledProfile = prepareLedProfileToBeSent(getLedProfileData());
      //Serial.print(ledProfile);
      //notifyClients(ledProfile);
    }

    // Else received data is LED profile ready to be parsed to JSON from converted data format
    else {
      /*JSONVar receivedLedProfile = JSON.parse((char*)data);

      Serial.print("\n");
      Serial.println((int) receivedLedProfile["red"]);
      Serial.println((int) receivedLedProfile["green"]);
      Serial.println((int) receivedLedProfile["blue"]);

      //ZMIENIĆ BIBLIOTEKĘ NP NA ARDUINOJSON7 BO TU TO SIE NAWET STRINGA W INTA ZAMIENIC NIE DA
      setLedColor((int) receivedLedProfile["red"], (int) receivedLedProfile["green"], (int) receivedLedProfile["blue"]);
    }
  }
} */
