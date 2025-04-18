#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid = "Ganti ssid wifi kamu";
const char* password = "ganti password wifi kamu";

WiFiServer server(80);
Servo myServo;

const int servoPin = 26; ///// ganti pin bebas
String header;

void setup() {
  Serial.begin(115200);

  myServo.setPeriodHertz(50);      // 50 Hz untuk servo analog
  myServo.attach(servoPin, 500, 2400); // Rentang PWM (0–180 derajat)

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.println("IP address: " + WiFi.localIP().toString());

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
          // HTTP Response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          // Kontrol servo
          if (header.indexOf("GET /BUKA") >= 0) {
            myServo.write(90); // Buka
          } else if (header.indexOf("GET /TUTUP") >= 0) {
            myServo.write(0);  // Tutup
          }

          // HTML dan CSS
          client.println(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Kontrol Servo ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin-top: 50px;
      background: #f7f7f7;
    }
    h1 {
      color: #444;
    }
    .button {
      padding: 16px 40px;
      font-size: 20px;
      margin: 10px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
    }
    .open {
      background-color: #4CAF50;
      color: white;
    }
    .close {
      background-color: #FF5722;
      color: white;
    }
  </style>
</head>
<body>
  <h1>Kontrol Servo ESP32</h1>
  <a href="/BUKA"><button class="button open">BUKA</button></a>
  <a href="/TUTUP"><button class="button close">TUTUP</button></a>
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
