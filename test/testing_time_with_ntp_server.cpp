#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

const char* ssid = "My ROS Server";
const char* password = "#ritu123";
time_t now;


void setup(){
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }

    configTime(0, 0, "pool.ntp.org");   // take some time to get the value
}

void loop(){
    time(&now);

    Serial.print("Unix Time: ");
    Serial.println((unsigned long)now);

    delay(1000);
}