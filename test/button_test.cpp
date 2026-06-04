#include<Arduino.h>
const int button1 = 15;
int status_1 = 0;
void setup(){
    Serial.begin(9600);
    pinMode(button1, INPUT);
}
void loop(){
    status_1 = digitalRead(button1);
    Serial.print("Status_1: ");
    Serial.println(status_1);
    delay(100);   // 10Hz
}