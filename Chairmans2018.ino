#include <Servo.h>
#include <FastLED.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"

// Comment out for prod to save memory
#define DEBUG 1

// Fan
//const int TALON_PWM_PIN = 10;
//int percent = 0;

// LEDs
const int LED_PIN = 6;
const int NUM_LEDS = 300;
const int LED_BRIGHTNESS = 50;

// Thermometer
const int THERMO_LEDS = 40;
const int THERMO_COLOR_R = 255;
const int THERMO_COLOR_G = 0;
const int THERMO_COLOR_B = 0;
int thermoLevel = 0;

// Trellis button pad
const int TRELLIS_INT_PIN = A2;
const int TRELLIS_NUM_KEYS = 16;

// Ballz servo
const int SERVO_PIN = 9;

// Stepper
const int STEPPER_ENABLE_PIN = 3;
const int STEPPER_PIN = 5;
const int STEPPER_DIR_PIN = 4;


Servo motor;
Servo ballServo;
CRGB leds[NUM_LEDS];
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("2018 Chairmans starting");
//    Serial.println("Start talon");
  #endif

  // Fan
//  motor.attach(TALON_PWM_PIN);
//  delay(100);
//  
  #ifdef DEBUG
//    Serial.println("Talon setup success");
    Serial.println("Start LED");
  #endif

  // LEDs
  FastLED.addLeds<NEOPIXEL,LED_PIN>(leds, NUM_LEDS);
  delay(500);
  setAll(0, 0, 0);
  delay(100);

  #ifdef DEBUG
    Serial.println("LED setup success");
    Serial.println("Start trellis");
  #endif

  // Trellis
  pinMode(TRELLIS_INT_PIN, INPUT);
  digitalWrite(TRELLIS_INT_PIN, HIGH);
  trellis.begin(0x70);
  delay(100);
  trellisBootLEDs();

  // Servo
  #ifdef DEBUG
    Serial.println("Start servo");
  #endif
  ballServo.attach(10);
  #ifdef DEBUG
    Serial.println("Startup success\n");
  #endif
}

void loop() {

  setThermo(0);
  delay(3000);
  setThermo(1);
  delay(3000);
  setThermo(2);
  delay(3000);
  setThermo(3);
  delay(3000);
  
//  int PWMvalue = percent * 5 + 1500; //scale up to 1000-2000
//
//  motor.writeMicroseconds(PWMvalue);

}

void enableStepper(bool isEnable) {
  if (isEnable) {
    // Set enable pin to low (disabled)
    digitalWrite(STEPPER_ENABLE_PIN, HIGH);
  } else {
    // Set enable pin to high (enable)
    digitalWrite(STEPPER_ENABLE_PIN, LOW);
  }
}

/**
 * Set the ballz servo
 * bool val - the value to set it to
 *  true - open
 *  false - close
 */
void setBallz(bool val) {
  
}

/**
 * Set the fan motor
 * int percent - percent to set the fan motor to
 *  -100 = full reverse
 *  0 = off
 *  100 = full forward
 */
void setFan(int percent) {
  if (percent < 0 || percent > 100) {
    Serial.println("setFan invalid percent");
    return;
  }
  // Scale up to 1000-2000
  int PWMvalue = percent * 5 + 1500;

  motor.writeMicroseconds(PWMvalue);
  Serial.print("Set fan PWM to ");
  Serial.println(PWMvalue);
}

void trellisBootLEDs() {
  for (uint8_t i=0; i<TRELLIS_NUM_KEYS; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
  for (uint8_t i=0; i<TRELLIS_NUM_KEYS; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
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

void setThermo(int level) {
  thermoLevel = level;
  if (thermoLevel > 4) {
    Serial.println("thermoLevel overflow!");
  }
  int levelHeight = THERMO_LEDS / 4;
  Serial.print("Set thermo level: ");
  Serial.println(levelHeight);
  setPortion(THERMO_COLOR_R, THERMO_COLOR_G, THERMO_COLOR_B, 0, (levelHeight * (level + 1)));
}

