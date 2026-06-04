#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "My ROS Server";
const char* password = "#ritu123";

void setup(){
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    }
    Serial.println(WiFi.localIP());
}
void loop(){
}