#include <Servo.h>
#include<FastLED.h>

int percent = 0;
const int TALON_PWM_PIN = 5;
const int NUM_LEDS = 300;
const int LED_PIN = 6;
const int LED_BRIGHTNESS = 50;

const int THERMO_LEDS = 40;

Servo motor;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  Serial.println("2018 Chairmans starting");
  Serial.println("Start talon");
  motor.attach(TALON_PWM_PIN);
  delay(1000);
  Serial.println("Talon setup success");
  Serial.println("Start LED");
  FastLED.addLeds<NEOPIXEL,LED_PIN>(leds, NUM_LEDS);
  delay(500);
  setAll(0, 0, 0);
  Serial.println("LED setup success");
  Serial.println("Startup success\n");
  delay(1000);
}

void loop() {

  setThermo(255, 0, 0, 0);
  delay(3000);
  setThermo(255, 0, 0, 1);
  delay(3000);
  setThermo(255, 0, 0, 2);
  delay(3000);
  
  int PWMvalue = percent * 5 + 1500; //scale up to 1000-2000

  motor.writeMicroseconds(PWMvalue);

//  if (Serial.available() > 1) {
//    long proposedValue = Serial.parseInt();
//    if (proposedValue == 0) {
//      Serial.println("Stopping");
//      percent = 0;
//    } else if (proposedValue >= -100 && proposedValue <= 100) {
//      percent = proposedValue;
//      Serial.print("Motor value set to ");
//      Serial.print(percent);
//      Serial.println("%");
//    } else {
//      Serial.println("Bad input");
//    }
//  }
}

void setAll(int r, int g, int b) {
  fill_solid( &(leds[0]), 300, CRGB( r, g, b) );
  FastLED.show();
}

void setPortion(int r, int g, int b, int start, int finish) {
  Serial.print(start);
  Serial.print(" | ");
  Serial.println(finish);
//  fill_solid( &(leds[start]), &(leds[finish]), CRGB( r, g, b) );
//  FastLED.show();
}

void setThermo(int r, int g, int b, int level) {
  int levelHeight = THERMO_LEDS / 4;
  Serial.println(levelHeight);
  setPortion(r, g, b, (levelHeight * level), (levelHeight * (level + 1)));
}

