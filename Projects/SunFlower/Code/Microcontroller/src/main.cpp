#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <WiFi.h>
#include "html_page.h"

#include <FastLED.h>

#define PUMP_PIN 15
#define MOISTURE_SENSOR_PIN 34
#define WATER_LEVEL_PIN 35
#define LED_STRIP_PIN 13
#define LEDS_NUMBER 22
#define LED1 27
#define LED2 26
#define LED3 25
#define LED4 33

CRGB leds[LEDS_NUMBER];

CRGB stripColor = (255, 255, 255);
int ledStripColor[3];
int brigthness = 128;
bool firstTimeON = true;

int displayLeds[4] = {LED1, LED2, LED3, LED4};

int moistureRawInterval[] = {1300, 3050};
int minSoilMoisture = 30;
int currentTimePump = 0, currentTimeWater = 0, previousTimePump = 0, previousTimeWater = 0;
int pumpErrorCounter = 0, waterErrorCounter = 0, pumpErrorCount = 7; // seconds
bool pumpError = false;
bool autoWatering = false;
bool watering = false;

int waterLevelRawInterval[] = {900, 2000};

const char* accessPointName = "MyPlantsConfig";
const char* accessPointPassword = "myplantsid";

DNSServer dnsServer;
AsyncWebServer server(80);

void EEPROMSetup(unsigned size) {
  EEPROM.begin(size);
}

void SaveCredentials(String wifiName, String wifiPassword) {
  EEPROM.writeString(0, wifiName);
  EEPROM.writeString(20, wifiPassword);
  EEPROM.commit();
}

void CreateAP() {
  WiFi.softAP(accessPointName, accessPointPassword);
  dnsServer.start(53, "*", WiFi.softAPIP());
  MDNS.begin("MyPlants");
}

void ConnectToWiFi(String ssid, String password) {
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 50) {
    delay(100);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("IP Address: " + WiFi.localIP().toString());
    WiFi.softAPdisconnect(true);
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }
}

void EEPROMCheck() {
  String storedWiFiName = EEPROM.readString(0);
  String storedWiFiPassword = EEPROM.readString(20);
  if (storedWiFiName.length() > 0 && storedWiFiPassword.length() > 0) {
    ConnectToWiFi(storedWiFiName, storedWiFiPassword);
  }
}

void PumpSetup(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void MoistureSensorSetup(int pin) {
  pinMode(pin, INPUT);
}

void WaterLevelSensorSetup(int pin) {
  pinMode(pin, INPUT);
}

void TurnONLedStrip(CRGB color, int brigthness) {
  for (int i = 0; i < LEDS_NUMBER; i++) {
    leds[i] = color;
    FastLED.setBrightness(brigthness);
    FastLED.show();
    delay(25); // to be changed (millis)
  }
}

void TurnOFFLedStrip() {
  for (int i = LEDS_NUMBER - 1; i >= 0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(25); // to be changed (millis)
  }
}

void ChangeStripBrightness(int brigthness) {
  for (int i = 0; i < LEDS_NUMBER; i++) {
    FastLED.setBrightness(brigthness);
    FastLED.show();
  }
}

void LedsSetup(int pins[4], int ledsNumber) {
  FastLED.addLeds<WS2812, LED_STRIP_PIN, GRB>(leds, LEDS_NUMBER);
  FastLED.setBrightness(brigthness);
  for (int i = 0; i < LEDS_NUMBER; i++) {
    leds[i] = CRGB::White;
    if(i % 5 == 0) {
      pinMode(pins[i / 5 - 1], OUTPUT);
      digitalWrite(pins[i / 5 - 1], HIGH);
    }
    FastLED.show();
    delay(25); // to be changed (millis)
  }
  TurnOFFLedStrip();
  for (int i = 0; i < ledsNumber; i++) {
    digitalWrite(pins[i], LOW);
  }
}

int Clamp(int rawValue, int minValue, int maxValue) {
  return max(minValue, min(rawValue, maxValue));
}

unsigned WaterLevelReading(int pin) {
  unsigned rawValue = analogRead(pin);
  unsigned clampedValue = Clamp(rawValue, waterLevelRawInterval[0], waterLevelRawInterval[1]);
  unsigned percentValue = 100 * (clampedValue - waterLevelRawInterval[0]) / (waterLevelRawInterval[1] - waterLevelRawInterval[0]);
  return percentValue;
}

unsigned MoistureReading(int pin) {
  unsigned rawValue = analogRead(pin);
  unsigned clampedValue = Clamp(rawValue, moistureRawInterval[0], moistureRawInterval[1]);
  unsigned percentValue = 100 * (moistureRawInterval[1] - clampedValue) / (moistureRawInterval[1] - moistureRawInterval[0]);
  return percentValue;
}

void Server() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", captivePortalPage);
  });

  server.on("/configure", HTTP_POST, [](AsyncWebServerRequest *request){
    String wifiName = request->arg("wifiName");
    String wifiPassword = request->arg("wifiPassword");
    SaveCredentials(wifiName, wifiPassword);
    ConnectToWiFi(wifiName, wifiPassword);
    request->send(200, "text/html", saveConfirmationPage);
  });

  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", captivePortalPage);
  });
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", captivePortalPage);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (WiFi.status() == WL_CONNECTED) {
      request->send(200, "text/plain", "Connected to: " + WiFi.localIP().toString());
    } else {
      request->send(200, "text/plain", "Not connected");
    }
  });

  server.on("/waterStatus", HTTP_GET, [](AsyncWebServerRequest *request){
    if (WiFi.status() == WL_CONNECTED) {
      request->send(200, "text/plain", String(WaterLevelReading(WATER_LEVEL_PIN)));
    }
  });

  server.on("/moistureStatus", HTTP_GET, [](AsyncWebServerRequest *request){
    if (WiFi.status() == WL_CONNECTED) {
      request->send(200, "text/plain", String(MoistureReading(MOISTURE_SENSOR_PIN)));
    }
  });

  server.on("/stripOnOff", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("ledStripState")) {
      String stripState = request->getParam("ledStripState")->value();
      if (stripState == "on") {
        if(firstTimeON) {
          TurnONLedStrip(CRGB::White, brigthness);
          firstTimeON = 0;
        } else {
          TurnONLedStrip(stripColor, brigthness);
        }
      } else if (stripState == "off") {
        TurnOFFLedStrip();
      }
    }
  });

  server.on("/pickedColor", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("stripColor")) {
      String stripColorString = request->getParam("stripColor")->value();
      int rgbArray[3], stringIndex = 0;
      for (int i = 0; i < 3; i++) {
        if(i == 0) {
          String tempString = stripColorString.substring(stringIndex, stripColorString.indexOf(','));
          rgbArray[i] = tempString.toInt();
        } else {
          String tempString = stripColorString.substring(stringIndex, stripColorString.indexOf((i >= 1 && i < 2) ? ' ' : '\0'));
          rgbArray[i] = tempString.toInt();
        }
        Serial.println(rgbArray[i]);
        if(rgbArray[i] < 10) {
          stringIndex += 2;
        } else if(rgbArray[i] > 9 && rgbArray[i] < 100) {
          stringIndex += 3;
        } else {
          stringIndex += 4;
        }
      }
      stripColor = CRGB(rgbArray[0], rgbArray[1], rgbArray[2]);
      TurnONLedStrip(stripColor, brigthness);
    }
  });

  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("brightnessValue")) {
      String brightnessValueString = request->getParam("brightnessValue")->value();
      brigthness = brightnessValueString.toInt();
      ChangeStripBrightness(brigthness);
    }
  });

   server.on("/autoWatering", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("autoState")) {
      String autoWateringString = request->getParam("autoState")->value();
      if (autoWateringString == "on") {
        autoWatering = true;
      } else if (autoWateringString == "off") {
        autoWatering = false;
      }
    }
  });

  server.on("/watering", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("wateringState")) {
      String wateringState = request->getParam("wateringState")->value();
      if (wateringState == "on") {
        watering = true;
        autoWatering = false;
      } else if (wateringState == "off") {
        watering = false;
      }
    }
  });

  server.on("/newAp", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("newAPState")) {
      String newApState = request->getParam("newApState")->value();
      if (newApState == "on") {
        CreateAP();
      }
    }
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  LedsSetup(displayLeds, 4);
  PumpSetup(PUMP_PIN);
  MoistureSensorSetup(MOISTURE_SENSOR_PIN);
  WaterLevelSensorSetup(WATER_LEVEL_PIN);
  EEPROMSetup(512);
  EEPROMCheck();
  CreateAP();
  Server();
}

void AutoWatering() {
  if(pumpError) {
      digitalWrite(PUMP_PIN, LOW);
      Serial.println("Pump error! Reset!");
      return;
  }
  if(digitalRead(PUMP_PIN)) {
    currentTimePump = millis();
    if(currentTimePump - previousTimePump >= 1000) {
        previousTimePump = currentTimePump;
        pumpErrorCounter++;
    }
    if(pumpErrorCounter >= pumpErrorCount) {
      pumpError = true;
    }
  } else {
    pumpErrorCounter = 0;
  }
  if(autoWatering) {
    int moisture = MoistureReading(MOISTURE_SENSOR_PIN);
    if(moisture < minSoilMoisture) {
      digitalWrite(PUMP_PIN, HIGH);
    } else {
      digitalWrite(PUMP_PIN, LOW);
    }
  } else {
    digitalWrite(PUMP_PIN, watering ? HIGH : LOW);
  }
}

void WaterLevel() {
  unsigned waterLevel = WaterLevelReading(WATER_LEVEL_PIN);
  if(waterLevel <= 10 || pumpError) {
    currentTimeWater = millis();
    if(currentTimeWater - previousTimeWater >= 500) {
      previousTimeWater = currentTimeWater;
      waterErrorCounter++;
    }
    if(waterErrorCounter % 2 == 0) {
      digitalWrite(displayLeds[0], HIGH);
      digitalWrite(displayLeds[1], pumpError ? LOW : HIGH);
      digitalWrite(displayLeds[2], pumpError ? LOW : HIGH);
      digitalWrite(displayLeds[3], HIGH);
    } else {
      digitalWrite(displayLeds[0], LOW);
      digitalWrite(displayLeds[1], pumpError ? HIGH : LOW);
      digitalWrite(displayLeds[2], pumpError ? HIGH : LOW);
      digitalWrite(displayLeds[3], LOW);
    }
  } else if(waterLevel > 10 && waterLevel <= 25) {
    digitalWrite(displayLeds[0], HIGH);
    digitalWrite(displayLeds[1], LOW);
    digitalWrite(displayLeds[2], LOW);
    digitalWrite(displayLeds[3], LOW);
  } else if(waterLevel > 25 && waterLevel <= 50) {
    digitalWrite(displayLeds[0], HIGH);
    digitalWrite(displayLeds[1], HIGH);
    digitalWrite(displayLeds[2], LOW);
    digitalWrite(displayLeds[3], LOW);
  } else if(waterLevel > 50 && waterLevel <= 75) {
    digitalWrite(displayLeds[0], HIGH);
    digitalWrite(displayLeds[1], HIGH);
    digitalWrite(displayLeds[2], HIGH);
    digitalWrite(displayLeds[3], LOW);
  } else if(waterLevel > 75) {
    digitalWrite(displayLeds[0], HIGH);
    digitalWrite(displayLeds[1], HIGH);
    digitalWrite(displayLeds[2], HIGH);
    digitalWrite(displayLeds[3], HIGH);
  }
}

void loop() {
  WaterLevel();
  AutoWatering();
  dnsServer.processNextRequest();
}