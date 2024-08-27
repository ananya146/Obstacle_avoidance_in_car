
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* host = "your_server_ip_or_domain";

const int trigPin = 9;
const int echoPin = 10;
const long maxDistance = 200; // Maximum distance to check in cm

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to Wi-Fi");
}

void loop() {
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < maxDistance) {
    sendAlert(distance);
  }

  delay(1000); // Checking every second 
}

void sendAlert(long distance) {
  WiFiClient client;
  
  if (!client.connect(host, 80)) {
    Serial.println("Connection to host failed");
    return;
  }

  String url = "/alert?distance=" + String(distance);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(500);
}
