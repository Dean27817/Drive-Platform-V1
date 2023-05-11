 #include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP32Servo.h>

int motor1Pin1 = 5; 
int motor1Pin2 = 4; 
int ENA2 =13;

int motor2Pin1 = 15; 
int motor2Pin2 = 16;  
int ENA1 = 14;

int armPin1 = 18;
int armPin2 = 19;

Servo leftMotor;
Servo rightMotor;

const int frequency = 500;
const int pwm_channel1 = 0;
const int resolution = 8;
const int pwm_channel2 = 1;


const char* ssid = "NETGEAR81";
const char* password = "wateryprairie587";
WiFiServer server(80);

void setup() {

  Serial.begin(115200);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(15, HIGH);
  digitalWrite(16, LOW);
  digitalWrite(14, LOW);
   digitalWrite(3, LOW);
   

  ledcSetup(pwm_channel1, frequency, resolution);
  ledcAttachPin(ENA1, pwm_channel1);
  ledcSetup(pwm_channel2, frequency, resolution);
  ledcAttachPin(ENA2, pwm_channel1);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  leftMotor.setPeriodHertz(50);
  rightMotor.setPeriodHertz(50);
  leftMotor.attach(armPin1, 500, 2400);
  rightMotor.attach(armPin2, 500, 2400);
  
  
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




void movement(char motor, bool direction, int espeed)
{
  if (motor == 'l')
  {
    ledcWrite(pwm_channel1, espeed);
    if (direction)
    {
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
    }
    else
    {
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
    }
  }
  else if (motor == 'r')
  {
    ledcWrite(pwm_channel2, espeed);
    if (direction)
    {
      digitalWrite(16, LOW);
      digitalWrite(15, LOW);
    }
    else
    {
      digitalWrite(16, HIGH);
      digitalWrite(15, HIGH);
    }
  }
  else
  {
     ledcWrite(pwm_channel2, espeed);
    ledcWrite(pwm_channel1, espeed);
     if (direction)
    {
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(16, LOW);
      digitalWrite(15, LOW);
    }
    else
    {
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(16, HIGH);
      digitalWrite(15, HIGH);
    }
  }
}












void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    int xpos = -1, ypos = -1;
    bool button = false;
    int espeed = 0;
    if (request.startsWith("GET /")) {
      int x = request.indexOf("x=");
      int y = request.indexOf("&y=");
      bool button = request.indexOf(", button state=");
      int espeed = request.indexOf(", speed =");
      espeed = espeed / 4.011;
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
    

    if (xpos > 100 || xpos < 100 || ypos > 100 || ypos < 100)
    {
      client.print("x=");
      client.print(xpos);
      client.print("&y=");
      client.print(ypos);
      client.println();
      
    if (xpos < 0)
    {
     movement('b', true, espeed);
    }
    else 
    {
      movement('b', false, espeed);
    }

    if  (ypos < 0)
    {
      movement('l', true, espeed);
      movement('r', false, espeed);
    }
    else
    {
      movement('r', true, espeed);
      movement('l', false, espeed);
    }
    }
    else
  {
    movement('b', false, 0);
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
