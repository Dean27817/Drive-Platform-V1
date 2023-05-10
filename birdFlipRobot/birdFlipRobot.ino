#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "NETGEAR81";
const char* password = "wateryprairie587";
WiFiServer server(80);

void setup() {
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
      
     }

    client.stop();
  }
}
