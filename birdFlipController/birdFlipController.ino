#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "NETGEAR81";
const char* password = "wateryprairie587";
const char* host = "192.168.0.11";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  pinMode(4, INPUT);
}

void loop() {
  WiFiClient client;
  if (client.connect(host, 80)) {
    int x = analogRead(34); // read joystick x value from ADC1 channel 6 (GPIO 34)
    int y = analogRead(35); // read joystick y value from ADC1 channel 7 (GPIO 35)
    int speed = analogRead(26);
    bool button = digitalRead(4);
    client.print("x=");
    client.print(x - 1900);
    client.print("&y=");
    client.print(y - 1900);
    client.print(", button state=");
    client.print(button);
    client.print(", speed =");
    client.print(speed);
    client.println(" HTTP/1.1 192.168.0.11");
    client.println("Host: ");
    client.println("Connection: close");
    client.println();
    Serial.print("Sent x=");
    Serial.print(x);
    Serial.print(", y=");
    Serial.print(y);
    Serial.print(", sent button = ");
    Serial.print(button);
    Serial.print(", speed=");
    Serial.println(speed);
    } else {
    Serial.println("Failed to connect to server");
  }
  delay(100); // send joystick values every 500 milliseconds
}
