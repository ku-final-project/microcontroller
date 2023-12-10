#include "config.h"

void readConfigFromSPIFFS(const char *filename, Config &config){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open(filename);

  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  String json;
  while (file.available()) {
    json = file.readString();
  }
  const char* jsonString = json.c_str();
  DynamicJsonDocument doc(200);
  deserializeJson(doc, jsonString);

  const char* ssid = doc["ssid"];
  const char* password = doc["password"];
  config.ssid = String(ssid);
  config.password = String(password);
  file.close();
}