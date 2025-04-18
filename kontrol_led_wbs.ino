#include <WiFi.h>

// Ganti dengan WiFi kamu
const char* ssid = "GANTI SSID WIFI KAMU";
const char* password = "GANTI PASSWORD WIFI KAMU";

WiFiServer server(80);
const int ledPin = 26; // Ganti ke pin lain juga boleh
String header;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          // Kirim response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          // Kontrol LED
          if (header.indexOf("GET /LED=ON") >= 0) {
            digitalWrite(ledPin, HIGH);
          } else if (header.indexOf("GET /LED=OFF") >= 0) {
            digitalWrite(ledPin, LOW);
          }

          // HTML dan CSS
          client.println(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LED Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin-top: 50px;
      background: #f2f2f2;
    }
    h1 {
      color: #333;
    }
    .button {
      padding: 16px 40px;
      font-size: 20px;
      margin: 10px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
    }
    .on {
      background-color: #4CAF50;
      color: white;
    }
    .off {
      background-color: #f44336;
      color: white;
    }
  </style>
</head>
<body>
  <h1>ESP32 LED Control</h1>
  <a href="/LED=ON"><button class="button on">LED ON</button></a>
  <a href="/LED=OFF"><button class="button off">LED OFF</button></a>
</body>
</html>
)rawliteral");

          client.println();
          break;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}
