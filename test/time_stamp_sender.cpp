// Here the data send over UDP with time stamp and status of button
#include <WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

const char* ssid = "SSID";
const char* password = "password";
const int button = 15;
int status = 0;

WiFiUDP udp;

IPAddress receiverIP(10,14,148,209);  // receiver ESP IP
const int receiverPort = 8888;

struct Packet {
    uint32_t timestamp;
    uint8_t button;
};
Packet pkt;

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");             
  }
  Serial.println("\nSender connected!");
  configTime(0, 0, "pool.ntp.org");   // take some time to get the value
  udp.begin(8888);   
}

void loop() {
  int newStatus = !digitalRead(button); 
  if (newStatus != status) {             // only send on change
    time_t now = time(nullptr);
    pkt.timestamp = (uint32_t)now;
    status = newStatus;
    pkt.button = newStatus;
    udp.beginPacket(receiverIP, receiverPort);
    udp.write((uint8_t*)&pkt, sizeof(pkt));
    udp.endPacket();

    Serial.print("Sent timestamp: ");
    Serial.println(pkt.timestamp);
  }
  delay(20);
}