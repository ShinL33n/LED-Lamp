#include "../headers/WebSocketHandler.h"

WebSocketHandler::WebSocketHandler()
{
  Serial.println("[WebSocketHandler]: Please use overloaded constructor for webSocket to work.");
}

WebSocketHandler::WebSocketHandler(AsyncWebServer *server, AsyncWebSocket *webSocket)
{
  _server = *server;
  _webSocket = *webSocket;
}

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
      HandleWebSocketMessages(arg, data, len);
      break;

    case WS_EVT_PONG:
        break;
    case WS_EVT_ERROR:
        Serial.printf("WebSocket event error occured.");
      break;
  }
}

void WebSocketHandler::HandleWebSocketMessages(void *arg, uint8_t *data, size_t len) {
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
      setLedColor((int) receivedLedProfile["red"], (int) receivedLedProfile["green"], (int) receivedLedProfile["blue"]);*/
    }
  }
}