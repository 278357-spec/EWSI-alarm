#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h> // 1. Include the mDNS library
#include <Arduino.h>

WebServer server(80);
const int actuatorPin = 2;

void handleActuator() {
  Serial.println("Webhook");
  digitalWrite(actuatorPin, HIGH);
  delay(500);
  digitalWrite(actuatorPin, LOW);
  server.send(200, "text/plain", "Actuator Triggered!");
}

void setup() {
  pinMode(actuatorPin, OUTPUT);
  Serial.begin(9600);
  WiFi.begin("MERCUSYS", "1234567890");

  while (WiFi.status() != WL_CONNECTED) delay(500);

   //2. Initialize mDNS
   //This sets the address to http://my-actuator.local
   if (!MDNS.begin("my-actuator")) {
     // Serial.println("Error setting up MDNS responder!");
   }

   server.on("/trigger", HTTP_POST, handleActuator);
   server.begin();

   // 3. Add service to mDNS (optional but good practice)
   MDNS.addService("http", "tcp", 80);
}

void loop() {
  // mDNS doesn't require a loop function on ESP32,
  // but the web server still does.
  server.handleClient();
  // digitalWrite(actuatorPin, HIGH);
  // delay(1000);
  //
  // digitalWrite(actuatorPin, LOW);
  // delay(1000);
}