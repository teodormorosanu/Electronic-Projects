#include <Wire.h>
#include "RTClib.h" // Library for real time clock

#define IN1 11 // MX1508 IN1 pin

RTC_PCF8563 rtc;

void RtcSetup() { // Method for RTC setup
  Wire.begin(); // Wire begin
  rtc.begin(); // Real time clock begin
  rtc.adjust(DateTime(__DATE__, __TIME__)); // Adjusting the RTC (computer date)
}

void PirSetup() { // Method for PIR setup
  pinMode(IN1, OUTPUT); // Setting the MX1508 IN
}

void setup () {
  RtcSetup(); // Method for RTC setup
  PirSetup(); // Method for PIR setup
}

void Pir() { // Method for powering the motor
  DateTime now = rtc.now(); // Getting the RTC time
  if(now.minute() == 59 && now.second() >= 55) { // At each hour, for 5 seconds, the motor spins
    digitalWrite(IN1, HIGH);
  } else {
    digitalWrite(IN1, LOW);
  }
}

void loop () {
  Pir(); // Method for powering the motor
}
