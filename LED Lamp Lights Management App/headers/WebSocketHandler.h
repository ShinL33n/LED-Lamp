#pragma once

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "LedProfileHandler.h"

class WebSocketHandler
{
private:
	AsyncWebServer _server;
	AsyncWebSocket _webSocket;
	LedProfileHandler _ledProfileHandler;

	void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);    // Handles different WebSocket events
	void HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);                                   // Handles messages received via WebSocket
	void HandleReceivedMessage(JsonDocument receivedDataJsonDocument, AsyncWebSocketClient *client);

public:
	WebSocketHandler(AsyncWebServer *server, AsyncWebSocket *webSocket, LedProfileHandler *ledProfileHandler);
	//WebSocketHandler(LedProfileHandler *ledProfileHandler);

	// Initialize WebSocket and a server for it
	void InitializeWebSocket();
};