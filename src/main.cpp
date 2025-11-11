#include <Arduino.h>
#include "Stepper.h"
#include <WiFi.h>

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -21600
#define UTC_OFFSET_DST 0

int hourminpin = 5;
int hourstep = 9;
int hourdir = 10;

int minminpin = 6;
int minstep = 11;
int mindir = 12;

int hours = 0;
float hourPos = 0;
int minutes = 0;
float minutePos = 0;

Stepper hour = Stepper(hourstep,hourdir);
Stepper minute = Stepper(minstep,mindir);

void setup() {
  Serial.begin(9600);
  while(!Serial);
  delay(1000);
  Serial.print("Connecting to WiFi");
  WiFi.begin("NSA Security Van HQ", "windowstothehallway", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.println("Hello, ESP32-S3!");
  hour.begin();
  hour.enable();
  Serial.println("Hello, hour motor!");
  minute.begin();
  minute.enable();
  Serial.println("Hello, minute motor!");

  delay(750);
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  delay(750);

  Serial.println("Time configurated");

  hour.homeBlocking();
  minute.homeBlocking();
}


void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Womp womp");
    return;
  }

  minutes = timeinfo.tm_min;
  minutePos = (minutes / 60.0) * 200.0;
  minutePos = minutePos * (72.0/10.0);

  hours = timeinfo.tm_hour;

  if (hours > 12) {
    hours = hours - 12;
  }
  hourPos = (hours / 12.0) * 200.0;

  minute.moveToBlocking(minutePos);
  hour.moveToBlocking(hourPos);
}
