#include "../headers/WebSocketHandler.h"

WebSocketHandler::WebSocketHandler(AsyncWebServer *server, AsyncWebSocket *webSocket)
  : _server(*server), _webSocket(*webSocket) {}


void WebSocketHandler::InitializeWebSocket() {
  _webSocket.onEvent(
      [this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
      this->OnEvent(server, client, type, arg, data, len);
    });
  _server.addHandler(&_webSocket);
}

void WebSocketHandler::OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;

    case WS_EVT_DATA:
      HandleWebSocketData(arg, data, len);
      break;

    case WS_EVT_ERROR:
        Serial.printf("WebSocket event error occured.");
      break;
  }
}

void WebSocketHandler::HandleWebSocketData(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    //String receivedMessage = (char*)data;
    HandleReceivedMessage((char*)data);
  }
}

// zaimplementować JSON'a bo wiadomości będę w jego formie (wartość type np doc["type"] == "getLedProfile" albo doc["type"] == "setLedProfile")
void WebSocketHandler::HandleReceivedMessage(char *message) {  
  if(strcmp(message, "test") == 0) {

  }
}