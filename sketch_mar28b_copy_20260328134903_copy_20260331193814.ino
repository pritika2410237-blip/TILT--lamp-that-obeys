#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define NUM_LEDS 8

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const int MPU = 0x68;
int16_t ax, ay, az;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  strip.begin();
  strip.show();
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  ax = Wire.read()<<8 | Wire.read();
  ay = Wire.read()<<8 | Wire.read();
  az = Wire.read()<<8 | Wire.read();

  if (abs(ax) > 20000 || abs(ay) > 20000) {
    flashLights();
  }
  else if (ax > 10000) {
    setColor(0, 0, 255);
  }
  else if (ax < -10000) {
    setColor(255, 0, 0);
  }
  else if (ay > 10000) {
    setColor(0, 255, 0);
  }
  else {
    setColor(20, 20, 20);
  }

  delay(100);
}

void setColor(int r, int g, int b) {
  for(int i=0; i<NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.show();
}

void flashLights() {
  for(int i=0; i<3; i++) {
    setColor(255,255,255);
    delay(100);
    setColor(0,0,0);
    delay(100);
  }
}