#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "My ROS Server";
const char* password = "#ritu123";
const int led = 26;

WiFiUDP udp;
const int localPort = 8888;
int status = 0;
unsigned long lastPacketTime = 0;
const unsigned long TIMEOUT_MS = 500;

time_t now;

struct Packet {
    uint32_t timestamp;
    uint8_t button;
};
Packet pkt;


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

  configTime(0, 0, "pool.ntp.org");   // take some time to get the value
}

void loop() {
  // --- Timeout: turn off LED if no packet received recently ---
  if (millis() - lastPacketTime > TIMEOUT_MS) {
    digitalWrite(led, LOW);
  }

  // --- Parse incoming UDP packet ---
  int len = udp.parsePacket();
  if (len == sizeof(pkt)) {
    udp.read((uint8_t*)&pkt, sizeof(pkt));
    Serial.print("Timestamp: ");
    Serial.println(pkt.timestamp);
    Serial.print("Status: ");
    Serial.println(pkt.button);

    if(pkt.button == 0x00 || pkt.button == 0x01){
      lastPacketTime = millis();           // reset timeout
      digitalWrite(led, pkt.button == 1 ? HIGH : LOW);
    }
  }
  else if(len > 0){
    Serial.println("Bad packet ignored");
    udp.flush();
  }
}