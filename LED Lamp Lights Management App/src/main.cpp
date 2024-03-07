#include <Arduino.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
// #include <chrono>
// #include <thread>
#include <ctime>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "../headers/LedProfileHandler.h"
#include "../headers/LedManager.h"

#define PIN 4
#define NUMPIXELS 8

Adafruit_NeoPixel LEDStrip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // Initialize NeoPixel LED Strip
LedProfile ledProfile;                                            // Led profile object used for memory profile storage
LedProfileHandler ledProfileHandler(&ledProfile);                 // Create handler for led profile to be able to manager led profile
AsyncWebServer server(80);                                        // Create AsyncWebServer object on port 80
AsyncWebSocket webSocket("/ws");                                  // Create a WebSocket object

void InitializeSPIFFS();         // Initialize file system for file storage
void ApplyLEDProfile();          // Initialize LEDs lighting and effects
void InitializeWiFiConnection(); // Initialize ESP32 connection to the network (option to log into desired network via web UI)
void InitializeCurrentTime();    // Initialize getting current time from the NTP

void InitializeWebSocket();                                                                                                  // Initialize WebSocket for data exchange lamp <-> client controller app                                                                                           // Initialize WebSocket for communication with client controller apps
void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len); // Handles different WebSocket events
void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);                                // Handles data received via WebSocket
void HandleReceivedMessage(JsonDocument receivedDataJsonDocument, AsyncWebSocketClient *client);                             // Deeper received messages handler

time_t countdownStartTime = time(0);
time_t currentTime;
double deltaMinutes = 5.0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup()
{
  Serial.begin(115200);

  InitializeSPIFFS();                         // Initialize file system
  ledProfileHandler.ReadLedProfileFromFile(); // Read LEDs profile on turn on or reset
  ApplyLEDProfile();                          // Apply LEDs profile to the LED strip
  InitializeWiFiConnection();                 // Initialize Wi-Fi connection manager
  InitializeCurrentTime();                    // Get current time with hard coded +1h time offset
  InitializeWebSocket();                      // Initialize Web Socket with messages handler
}

void loop()
{
  // Check every five minutes if it is the time to turn LEDs on/off
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }

  if (deltaMinutes >= 5)
  {
    uint8_t currentHour = timeClient.getHours();
    uint8_t currentMinutes = timeClient.getMinutes();

    Serial.print("[Clock]: ");
    Serial.print(currentHour);
    Serial.print(":");
    Serial.print(currentMinutes);
    Serial.print("\n");

    if ((currentHour > ledProfile.getStartHour() && currentHour < ledProfile.getEndHour()) ||
        (currentHour == ledProfile.getStartHour() && currentMinutes >= ledProfile.getStartMinutes()) ||
        (currentHour == ledProfile.getEndHour() && currentMinutes <= ledProfile.getEndMinutes()))
    {
      LedManager LEDs(&LEDStrip, &ledProfile);
      LEDs.On();
      // ledProfile.setLastState(true); POMYŚLEĆ JAK DOBRZE TO ZAIMPLEMENTOWAĆ ŻEBY LEDY NIE MIGAŁY PRZY STARCIE / RESTARCIE
    }
    else
    {
      LedManager LEDs(&LEDStrip, &ledProfile);
      LEDs.Off();
      // ledProfile.setLastState(false);
    }

    countdownStartTime = time(0);
  }

  currentTime = time(0);
  deltaMinutes = difftime(currentTime, countdownStartTime) / 60.0;

  delay(10000); // Loop every 10 sec to check time
}

void InitializeSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("[SPIFFS]: An error has occurred while mounting SPIFFS.");
  }
  Serial.println("[SPIFFS]: SPIFFS mounted successfully.");
}

void InitializeCurrentTime()
{
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}

void InitializeWiFiConnection()
{
  WiFi.mode(WIFI_STA);

  WiFiManager wifiManager;

  bool wifiResult = wifiManager.autoConnect("LED Lamp WiFi connection configuration", "ledpassword");

  if (!wifiResult)
  {
    Serial.printf("[WiFiManager]: Failed to connect to %s. \n", wifiManager.getWiFiSSID());
  }
  else
  {
    Serial.printf("[WiFiManager]: Connected to desired WiFi (SSID: %s). \n", wifiManager.getWiFiSSID());
  }
}

void ApplyLEDProfile()
{
  LedManager LEDs(&LEDStrip, &ledProfile); // Manages led strip effects
  LEDs.On();
}

void InitializeWebSocket()
{
  webSocket.onEvent(OnEvent);
  server.addHandler(&webSocket);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.serveStatic("/", SPIFFS, "/");

  server.begin();
}

void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("[WebSocketHandler]: WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;

  case WS_EVT_DISCONNECT:
    Serial.printf("[WebSocketHandler]: WebSocket client #%u disconnected \n", client->id());
    break;

  case WS_EVT_DATA:
    Serial.printf("[WebSocketHandler]: WebSocket client #%u sent: %s \n", client->id(), (char *)data);
    HandleWebSocketData(arg, data, len, client);
    break;

  case WS_EVT_ERROR:
    Serial.printf("[WebSocketHandler]: WebSocket event error occured.");
    break;
  }
}

void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;

    JsonDocument receivedJsonDocument;

    // Parse received data into JSON document
    DeserializationError deserializationError = deserializeJson(receivedJsonDocument, (char *)data);

    if (deserializationError)
    {
      Serial.printf("[WebSocketHandler]: Parse operation has failed trying to read led profile from incoming data (%s). \n", deserializationError.f_str());
    }
    else
    {
      HandleReceivedMessage(receivedJsonDocument, client);
    }
  }
}

void HandleReceivedMessage(JsonDocument receivedJsonDocument, AsyncWebSocketClient *client)
{
  if (receivedJsonDocument["Type"] == "getLedProfile")
  {
    webSocket.text(client->id(), ledProfileHandler.GetLedProfileToJsonString());
  }

  else if (receivedJsonDocument["Type"] == "setLedProfile")
  {
    ledProfileHandler.SetLedProfileFromJsonFormat(receivedJsonDocument);
    ApplyLEDProfile();
  }

  else
  {
    Serial.print("[WebSocketHandler]: Unsupported message received.");
  }
}
