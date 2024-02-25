#include "../headers/WebSocketHandler.h"

// CRASH IN CONSTRUCTOR \/
WebSocketHandler::WebSocketHandler(AsyncWebServer *server, AsyncWebSocket *webSocket, LedProfileHandler *ledProfileHandler)
  : _server(server), _webSocket(webSocket), _ledProfileHandler(ledProfileHandler) {

  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server->serveStatic("/", SPIFFS, "/");

  server->begin();
}

void WebSocketHandler::InitializeWebSocket() {
  _webSocket->onEvent(
      [this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
      this->OnEvent(server, client, type, arg, data, len);
    });
  _server->addHandler(_webSocket);
}

void WebSocketHandler::OnEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[WebSocketHandler]: WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("[WebSocketHandler]: WebSocket client #%u disconnected\n", client->id());
      break;

    case WS_EVT_DATA:
      HandleWebSocketData(arg, data, len, client);
      break;

    case WS_EVT_ERROR:
      Serial.printf("[WebSocketHandler]: WebSocket event error occured.");
      break;
  }
}

void WebSocketHandler::HandleWebSocketData(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

	  JsonDocument receivedJsonDocument;
    
    DeserializationError error = deserializeJson(receivedJsonDocument, (char*)data);
    Serial.println();

  Serial.printf("[HandleWebSocketData]: %s \n", (char*)data);

    if (error) {
      Serial.print("[WebSocketHandler]: Parsing failed:");
      Serial.println(error.f_str());
    }
    else {
      HandleReceivedMessage(receivedJsonDocument, client);
    }
  }
}

// zaimplementować JSON'a bo wiadomości będę w jego formie (wartość type np doc["type"] == "getLedProfile" albo doc["type"] == "setLedProfile")
void WebSocketHandler::HandleReceivedMessage(JsonDocument receivedJsonDocument, AsyncWebSocketClient *client) {  
  if(strcmp(receivedJsonDocument["Type"], "getLedProfile") == 0) {
    
  Serial.println("[HandleReceivedMessage]: XD z getLedProfile");
    String lol = _ledProfileHandler->GetLedProfileToJsonString(); // DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG 
    _webSocket->text((uint32_t)client, lol); /////////////////////////////////////////////////////////////////////////////////  <--------------------- ERROR TUTAJ
    // musimy uzyskać json format profilu, bo tak się posługujemu w programie dla wygody, przekonwertować go na stringa i podać w wiadomość
  }

  else if(strcmp(receivedJsonDocument["Type"], "setLedProfile") == 0) {
    Serial.println("[HandleReceivedMessage]: XD z setLedProfile"); // DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG DEBBUG 
    _ledProfileHandler->SetLedProfileFromJsonFormat(receivedJsonDocument);
    // tutaj odwrotnie, musimy przekonwertować stringa w jsona i wysłać do zapisania do pliku
        // przydała by się też wiadomość że plik zostać zapisany i można go odczytać z pliku jako nowy profil żeby nie robić tego za każdym razem kiedy potrzebny jest profil
        // MAMY JSON'a / JSON Stringa
        // musimy móc zapisać profil używając JSONA
        // muimy móc odczytać profil w formie stringa używając JSONA 
  }

  else {
    Serial.print("[WebSocketHandler]: Unsupported message.");
  }
}