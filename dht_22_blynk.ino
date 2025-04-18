#define BLYNK_TEMPLATE_ID "TMPL6M8sSSblz"
#define BLYNK_TEMPLATE_NAME "DHT 22"
#define BLYNK_AUTH_TOKEN "VjUHNVCjqfJMemUyv94Qwy-NjC0rYw-F"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Sahroni";
char pass[] = "nursahroni";

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
