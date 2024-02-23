#pragma once

#include <ESPAsyncWebServer.h>

class WebSocketHandler
{
private:
	AsyncWebServer _server;
	AsyncWebSocket _webSocket;

	void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);    // Handles different WebSocket events
	void HandleWebSocketData(void *arg, uint8_t *data, size_t len);                                                            	// Handles messages received via WebSocket
	void HandleReceivedMessage(char *message);

public:
	WebSocketHandler(AsyncWebServer *server, AsyncWebSocket *webSocket);

	// Initialize WebSocket and a server for it
	void InitializeWebSocket();

};