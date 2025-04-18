#define BLYNK_TEMPLATE_ID "id template"
#define BLYNK_TEMPLATE_NAME "nama templat"
#define BLYNK_AUTH_TOKEN "ganti token otentikasi"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "ssid wifi kamu";
char pass[] = "password wifi kamu";

#define DHTPIN 4          // Pin data DHT
#define DHTTYPE DHT22     // Ganti ke DHT11 jika pakai DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, t); // Temperature
  Blynk.virtualWrite(V1, h); // Humidity

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print("°C  Humidity: ");
  Serial.print(h);
  Serial.println("%");
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(2000L, sendSensor); // Setiap 2 detik
}

void loop() {
  Blynk.run();
  timer.run();
}
