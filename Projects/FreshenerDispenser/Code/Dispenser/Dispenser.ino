#include <Wire.h>
#include "RTClib.h"

RTC_PCF8563 rtc;
int motorIn = 11; // MX1508 IN1

void setup () {
  Wire.begin(); // Wire begin
  rtc.begin(); // Real time clock begin
  rtc.adjust(DateTime(__DATE__, __TIME__)); // Adjusting the RTC (computer date)
  pinMode(motorIn, OUTPUT); // Setting the motorIn pin
}

void loop () {
  DateTime now = rtc.now(); // Getting the RTC time
  if(now.minute() == 59 && now.second() >= 55) { // At each hour, for 5 seconds, the motor spins
    digitalWrite(motorIn, HIGH);
  } else {
    digitalWrite(motorIn, LOW);
  }
}
