#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "vm_smart_home";
const char* password = "DX9!PcxjscejR&GY";
const char* hostData = "http://192.168.4.1/post_data";
const char* hostDevice = "http://192.168.4.1/post_device";
const int port = 8000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  StaticJsonDocument<128> doc;
  JsonObject sensors = doc.createNestedObject("sensors");
  String jsonString;
  WiFiClient client;
  HTTPClient http;
  

  doc["device"] = WiFi.macAddress();
  doc["type"] = "Home Garden";
  sensors["DHT_22_TEMP"] = "Temperature";
  sensors["DHT_22_HUM"] = "Humidity";
  sensors["SOIL_1"] = "Soil Moisture";
  sensors["SOIL_2"] = "Soil Moisture";
  sensors["SOIL_3"] = "Soil Moisture";

  while (!http.begin(client, hostDevice)) {  // HTTP
    delay(10000);
  }

  http.addHeader("Content-Type", "application/json");
  serializeJson(doc, jsonString);
  int httpCode = http.POST(jsonString);

  http.end();
}

void loop() {
  if (!Serial.available()) {
    delay(10000);
    return;
  }

  String jsonString = Serial.readString();

  WiFiClient client;

  HTTPClient http;

  if (http.begin(client, hostData)) {  // HTTP
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(jsonString);

    http.end();
  }

  delay(1200000);
}