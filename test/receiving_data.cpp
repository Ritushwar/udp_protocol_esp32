#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "My ROS Server";
const char* password = "#ritu123";
const int led = 26;

WiFiUDP udp;
const int localPort = 8888;
char incomingPacket[11];   // 10 data bytes + 1 null terminator
int status = 0;
unsigned long lastPacketTime = 0;
const unsigned long TIMEOUT_MS = 500;

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
  // --- Timeout: turn off LED if no packet received recently ---
  if (millis() - lastPacketTime > TIMEOUT_MS) {
    digitalWrite(led, LOW);
  }

  // --- Parse incoming UDP packet ---
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 10);  // read max 10 bytes
    if (len > 0) {
      incomingPacket[len] = '\0';            // safe: array is size 11

      // Sanity check: only accept '0' or '1'
      if (incomingPacket[0] == '0' || incomingPacket[0] == '1') {
        status = atoi(incomingPacket);
        lastPacketTime = millis();           // reset timeout

        Serial.print("Received: ");       
        Serial.println(status);

        digitalWrite(led, status == 1 ? HIGH : LOW);
      } else {
        Serial.println("Bad packet ignored");
      }
    }
  }
}