#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "SSID";
const char* password = "password";
const int button = 15;
int status = 0;

WiFiUDP udp;

IPAddress receiverIP(10,14,148,209);  // receiver ESP IP
const int receiverPort = 8888;

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");             
  }
  Serial.println("\nSender connected!");
  udp.begin(8888);   
}

void loop() {
  int newStatus = !digitalRead(button); 
  if (newStatus != status) {             // only send on change
    status = newStatus;
    udp.beginPacket(receiverIP, receiverPort);
    udp.print(status);
    udp.endPacket();
  }
  delay(20);
}