#pragma once

#include <ESPAsyncWebServer.h>

// AsyncWebServer server(80);          // Create AsyncWebServer object on port 80
// AsyncWebSocket webSocket("/ws");    // Create a WebSocket object

class WebSocketHandler
{
private:

	void OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);    // Handles different WebSocket events
    void HandleWebSocketMessages(void *arg, uint8_t *data, size_t len);                                                             // Handles messages received via WebSocket

public:

	WebSocketHandler(AsyncWebServer *server, AsyncWebSocket *webSocket);

};