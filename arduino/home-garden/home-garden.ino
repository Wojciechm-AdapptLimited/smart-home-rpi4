#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <CircularBuffer.h>

#define SOIL_PIN_1 A0
#define SOIL_PIN_2 A1
#define SOIL_PIN_3 A2
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define ARR_SIZE 20

DHT_Unified dht(DHT_PIN, DHT_TYPE);

CircularBuffer<float, ARR_SIZE> temperature;
CircularBuffer<float, ARR_SIZE> humidity;
CircularBuffer<float, ARR_SIZE> soilMoisture1;
CircularBuffer<float, ARR_SIZE> soilMoisture2;
CircularBuffer<float, ARR_SIZE> soilMoisture3;

const int AirValue = 840;
const int WaterValue = 180;
const int intervals = (AirValue - WaterValue) / 3;

const char* moistureLevels[] = {"Too wet", "Very wet", "Wet", "Dry", "Too dry"};

unsigned long previousMillis = 0;
const unsigned long sampleInterval = 500;

StaticJsonDocument<128> doc;
JsonObject sensors = doc.createNestedObject("sensors");

void setup() {
  Serial.begin(115200);
  dht.begin();

  doc["device"] = "c8:c9:a3:54:b1:ad";
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis < sampleInterval) {
    return;
  }

  previousMillis = currentMillis;

  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temperature.push(event.temperature);
  }

  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    humidity.push(event.relative_humidity);
  }

  soilMoisture1.push(analogRead(SOIL_PIN_1));
  soilMoisture2.push(analogRead(SOIL_PIN_2));
  soilMoisture3.push(analogRead(SOIL_PIN_3));

  if (soilMoisture1.isFull()) {
    float avgTemperature = average(temperature);
    float avgHumidity = average(humidity);
    float avgSoilMoisture1 = average(soilMoisture1);
    float avgSoilMoisture2 = average(soilMoisture2);
    float avgSoilMoisture3 = average(soilMoisture3);

    sensors["DHT_22_TEMP"] = avgTemperature;
    sensors["DHT_22_HUM"] = avgHumidity;
    sensors["SOIL_1"] = getMoistureLevel(avgSoilMoisture1);
    sensors["SOIL_2"] = getMoistureLevel(avgSoilMoisture2);
    sensors["SOIL_3"] = getMoistureLevel(avgSoilMoisture3);

    temperature.clear();
    humidity.clear();
    soilMoisture1.clear();
    soilMoisture2.clear();
    soilMoisture3.clear();

    ArduinoJson6200_1::serializeJson(doc, Serial);
  }
}

const char* getMoistureLevel(float value) {
  int level = value / intervals;
  level = constrain(level, 0, 4);
  return moistureLevels[level];
}

float average(CircularBuffer<float, ARR_SIZE>& buffer) {
  float avg = 0.0;

  using index_t = CircularBuffer<float, ARR_SIZE>::index_t;
  for (index_t i = 0; i < buffer.size(); i++) {
    avg += buffer[i] / (float)buffer.size();
  }

  return avg;
}