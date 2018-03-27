
#include <Servo.h>
#include <FastLED.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"

// Comment out for prod to save memory
#define DEBUG 1

// LEDs
const int LED_PIN = 6;
const int NUM_LEDS = 300;
const int LED_BRIGHTNESS = 50;

// Thermometer
const int THERMO_LEDS = 40;
const int THERMO_START = 100; // First thermo led
const int THERMO_COLOR_R = 255;
const int THERMO_COLOR_G = 0;
const int THERMO_COLOR_B = 0;
int thermoLevel = 0;

// Trellis button pad
const int TRELLIS_INT_PIN = A2;
const int TRELLIS_NUM_KEYS = 16;

// Ballz servo
const int SERVO_PIN = 9;
const int SERVO_OPEN = 180;
const int SERVO_CLOSE = 0;
bool currentServo = false;

// Stepper
const int STEPPER_ENABLE_PIN = 3;
const int STEPPER_PIN = 5;
const int STEPPER_DIR_PIN = 4;
int currentStepper = 0;

Servo ballServo;
CRGB leds[NUM_LEDS];
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("2018 Chairmans starting");
    Serial.println("Start LED");
  #endif

  // LEDs
  FastLED.addLeds<NEOPIXEL,LED_PIN>(leds, NUM_LEDS);
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

  // Stepper
  #ifdef DEBUG
    Serial.println("Start trellis success");
    Serial.println("Start stepper");
  #endif
  pinMode(6,OUTPUT); // Enable
  pinMode(5,OUTPUT); // Step
  pinMode(4,OUTPUT); // Dir
  #ifdef DEBUG
    Serial.println("Stepper setup success");
    Serial.println("Startup success\n");
  #endif
}

void loop() {

  switch(checkTrellis()) {
    // First row
    case 0: // Turn to Ignition
      setStepper(1);
    break;
    case 1: // Thermo level 1
      setThermo(0);
    break;
    case 2: // Turns to Creativity && Thermo level 2 && Fan dial 1
      setThermo(1);
      setStepper(1);
    break;
    case 3: // Turn to Oppertunity && Thermo level 3 && Fan dial 2
      setThermo(2);
      setStepper(1); 
    break;

    // Second row
    case 4: // Turn to Interaction && Thermo level 4 && Fan dial 3
      setThermo(3);
      setStepper(1);
    break;
    case 5: // Turn to Spread && pull bolt
      setStepper(1);
    break;
    case 6: // Turn to blank
      setStepper(1);
    break;
  }
}

/**
 * Set the stepper motor to rotate
 * int side - side to display
 *  0-5 for the six sided display
 */
void setStepper(int side) {
  if (side > 5) {
    side = 0;
    currentStepper = 0;
  }
  // 200 steps = 1 rev
//  Serial.println("Loop 200 steps (1 rev)");
  side = map(side, 0, 5, 0, 200);
  enableStepper(true);
  for(int x = 0; x < side; x++) // Loop 200 times
  {
    digitalWrite(STEPPER_PIN,HIGH); // Output high
    delay(10); // Wait
    digitalWrite(STEPPER_PIN,LOW); // Output low
    delay(100); // Wait
  }
  enableStepper(false);
}

/**
 * Check the value of the trellis
 * return - button that was pressed (if any)
 *  0-15 for a single trellis board
 */
int checkTrellis() {
  if (trellis.readSwitches()) {
    // go through every button
    for (uint8_t i=0; i<TRELLIS_NUM_KEYS; i++) {
      // if it was pressed, turn it on
      if (trellis.justPressed(i)) {
        trellis.setLED(i);
        trellis.writeDisplay();
        return i;
      }
    }
  }
}

/**
 * Enable or disable the stepper
 * bool isEnable - if the stepper should be enabled
 *  true - enable
 *  false - disable
 */
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
 * Boot LEDs on trelis
 */
void trellisBootLEDs() {
  for (uint8_t i=0; i<TRELLIS_NUM_KEYS; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(50);
  for (uint8_t i=0; i<TRELLIS_NUM_KEYS; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
}

/**
 * Set all LEDs to one color
 * int r, g, b - color
 */
void setAll(int r, int g, int b) {
  fill_solid( &(leds[0]), 300, CRGB( r, g, b) );
  FastLED.show();
}

/**
 * Set a prtion of LEDs to a color
 * int r, g, b - color
 * int start - first LED
 * int finish - last led
 */
void setPortion(int r, int g, int b, int start, int finish) {
  Serial.print(start);
  Serial.print(" | ");
  Serial.println(finish);
  fill_solid( &(leds[start]), &(leds[finish]), CRGB( r, g, b) );
  FastLED.show();
}

/**
 * Set the thermometer LEDs
 * int level - level to set
 *  0 to 3 for a total of four levels
 */
void setThermo(int level) {
  thermoLevel = level;
  if (thermoLevel > 3) {
    Serial.println("thermoLevel overflow!");
  }
  int levelHeight = THERMO_LEDS / 4;
  Serial.print("Set thermo level: ");
  Serial.println(levelHeight);
  setPortion(THERMO_COLOR_R, THERMO_COLOR_G, THERMO_COLOR_B, THERMO_START, (levelHeight * (level + 1)));
}
