// In this testing i connect esp32 with ros2 node using udp protocol and receive a text message
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "My ROS Server";
const char* password = "#ritu123";
const int led = 26;

WiFiUDP udp;
const int localPort = 8888;
char incomingPacket[255];


void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("Receiver IP: ");
  Serial.println(WiFi.localIP());  // note this IP — put it in sender

  udp.begin(localPort);
  Serial.println("UDP listening on port 8888");
}

void loop() {
  int packetSize = udp.parsePacket();
   if (packetSize){
        int len = udp.read(incomingPacket, 254);

        if (len > 0){
            incomingPacket[len] = '\0';
            Serial.print("Received: ");
            Serial.println(incomingPacket);
            for(int i =0; i<len;i++){
                digitalWrite(led, HIGH);
                delay(100);
                digitalWrite(led, LOW);
                delay(100);
            }
        }
    }
}