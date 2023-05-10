#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP32Servo.h>

int motor1Pin1 = 12; 
int motor1Pin2 = 13; 
int enable1Pin = 14; 

int motor2Pin1 = 15; 
int motor2Pin2 = 16; 
int enable2Pin = 17; 

int armPin1 = 18;
int armPin2 = 19;

Servo leftMotor;
Servo rightMotor;


const char* ssid = "NETGEAR81";
const char* password = "wateryprairie587";
WiFiServer server(80);

void setup() {

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	leftMotor.setPeriodHertz(50);
  rightMotor.setPeriodHertz(50);
  leftMotor.attatch(armPin1, 500, 2400);
  rightMotor.attatch(armPin2, 500, 2400);
  
  Serial.begin(115200);
  Serial.println("connected");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    int xpos = -1, ypos = -1;
    bool button = false;
    int speed = 0;
    if (request.startsWith("GET /")) {
      int x = request.indexOf("x=") - 1895;
      int y = request.indexOf("&y=") - 1895;
      bool button = request.indexOf(", button state=");
      int speed = request.indexOf(", speed =");
      if (xpos != -1 && ypos != -1) {
        xpos = request.substring(xpos + 2, ypos).toInt();
        ypos = request.substring(ypos + 3, request.length() - 9).toInt();
      }
    }
    // Send the x and y values to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    

    if (xpos > 5 || xpos < 5 || ypos > 5 || ypos < 5)
    {
      client.print("x=");
      client.print(xpos);
      client.print("&y=");
      client.print(ypos);
      client.println();
      
    int xSpeed = xpos + speed;
    int ySpeed = ypos + speed;
      
    }
    if (button)
    {
      rightMotor.write(100);
      leftMotor.write(100);
    }
    else
    {
      rightMotor.write(100);
      leftMotor.write(100);      
    }

    client.stop();
  }
}
