#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define SOILPIN1 A0
#define SOILPIN2 A1
#define SOILPIN3 A2
#define DHTPIN 2
#define DHTTYPE DHT22
#define DELAY 1000
#define ARRSIZE 20

DHT_Unified dht(DHTPIN, DHTTYPE);

float temperature[ARRSIZE];
float humidity[ARRSIZE];
float soilMoisure1[ARRSIZE];
float soilMoisure2[ARRSIZE];
float soilMoisure3[ARRSIZE];

int idx = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);

  if (!isnan(event.temperature)) {
    temperature[idx] = event.temperature;
  }

  if (!isnan(event.relative_humidity)) {
    humidity[idx] = event.relative_humidity;
  }

  soilMoisure1[idx] = analogRead(SOILPIN1);
  soilMoisure2[idx] = analogRead(SOILPIN2);
  soilMoisure3[idx] = analogRead(SOILPIN3);

  idx++;

  if (idx == ARRSIZE) {
    Serial.println(avg(temperature, ARRSIZE));
    Serial.println(avg(humidity, ARRSIZE));
    Serial.println(avg(soilMoisure1, ARRSIZE));
    Serial.println(avg(soilMoisure2, ARRSIZE));
    Serial.println(avg(soilMoisure3, ARRSIZE));

    idx = 0;      
  }

  delay(DELAY);
}


int argMin(float* arr, int arrSize) {
  int imin = 0;
  float min = arr[0];

  for (int i = 0; i < arrSize; i++) {
    if (arr[i] < min) {
      min = arr[i];
      imin = i;
    }
  }

  return imin;
}

int argMax(float* arr, int arrSize) {
  int imax = 0;
  float max = arr[0];

  for (int i = 0; i < arrSize; i++) {
    if (arr[i] < max) {
      max = arr[i];
      imax = i;
    }
  }

  return imax;
}

float avg(float* arr, int arrSize) {
  int imin = argMin(arr, arrSize);
  int imax = argMax(arr, arrSize);
  float total = 0.0;

  for (int i = 0; i < arrSize; i++) {
    if (i == imin || i == imax) {
      total += arr[i];
    }
  }

  if (imin == imax) {
    return total/(arrSize - 1);    
  }

  return total/(arrSize - 1);  
}
