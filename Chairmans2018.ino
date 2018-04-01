
#include <FastLED.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"

// Comment out for prod to save memory
#define DEBUG 1

int loopi = 0;

// LEDs
const int LED_PIN = 6;
int isFire = false;
const int LED_START = 31;
const int NUM_LEDS = 300; // TODO: figure this out

// Hex LEDs
const int HEX_LED_START = 110;  // TODO: figure this out
const int NUM_HEX_LEDS = 20;
const int HEX_LED_R = 255;
const int HEX_LED_G = 255;
const int HEX_LED_B = 255;

// Thermometer
const int THERMO_START = 0; // First thermo led
const int THERMO_LEVELS[] = {3, 10, 17, 24, 30};
const int THERMO_DELAY = 100; // Delay for slide effect
const int THERMO_COLOR_R = 255;
const int THERMO_COLOR_G = 0;
const int THERMO_COLOR_B = 0;
int thermoLevel = 0;

// Trellis button pad
const int TRELLIS_INT_PIN = A3;
const int TRELLIS_NUM_KEYS = 16;

// Stepper
const int STEPPER_ENABLE_PIN = 3;
const int STEPPER_PIN = 5;
const int STEPPER_DIR_PIN = 4;
int currentStepper = 0;

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
  pinMode(STEPPER_ENABLE_PIN,OUTPUT); // Enable
  pinMode(STEPPER_PIN,OUTPUT); // Step
  pinMode(STEPPER_DIR_PIN,OUTPUT); // Dir
  #ifdef DEBUG
    Serial.println("Stepper setup success");
    Serial.println("Startup success\n");
  #endif
}

void loop() {
  loopi++;
  switch(checkTrellis()) {
    // First row
    case 0: // Turn to Ignition
      setStepper(1);
      setHexLEDs(true);
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
      // Set case lights to fire
      setThermo(3);
      setStepper(1);
      isFire = true;
      fireLEDs(false);
    break;
    case 5: // Turn to Spread && pull bolt
      setStepper(1);
      isFire = true;
    break;
    case 6: // Turn to blank
      setStepper(1);
    break;

    // Reset button
    case 15:
      // TODO: reset stepper, turn off LEDs
      setHexLEDs(false);
    break;
  }
  if (isFire && loopi % 20 == 0) {
    fireLEDs(true);
  }
}

/**
 * Set the background LEDs to fire
 */
void fireLEDs(bool showUpdate) {
  for (int i = LED_START; i < HEX_LED_START; i++) {
    int set = random(0, 100);
    if (set > 10) {
      // 90% chance to turn on LED with random fire color
      leds[i] = getRandomFireColor();
      if (showUpdate) {
        FastLED.show();
      }
    } else {
      // 10% chance for full red
      leds[i].setRGB(random(180, 255), 0, 0);
      if (showUpdate) {
        FastLED.show();
      }
    }
   FastLED.show();
  }
}

/**
 * Returns a CRGB color for fire
 */
CRGB getRandomFireColor() {
  int r = random(70, 255);
  int g = random(0, 70);
  int b = random(0, 35);
  return CRGB(r, g, b);
}

/**
 * Set the hex LEDs
 * bool on
 *  true = LEDs on
 *  false = LEDs off
 */
void setHexLEDs(bool on) {
  if (on) {
    // Turn on
    setPortion(HEX_LED_R, HEX_LED_G, HEX_LED_B, HEX_LED_START, NUM_LEDS);
  } else {
    // Turn off
    setPortion(0, 0, 0, HEX_LED_START, NUM_LEDS);
  }
}

/**
 * Set the stepper motor to rotate
 * int side - side to display
 *  0-5 step for the six sided display
 */
void setStepper(int side) {
  // 200 steps = 1 rev
  // One step = about 33 steps
  currentStepper++;
  currentStepper = currentStepper % 6;
  int steps = 33 * side;
  enableStepper(true);
  for(int x = 0; x < steps; x++) {
    digitalWrite(STEPPER_PIN,HIGH);
    delay(10);
    digitalWrite(STEPPER_PIN,LOW);
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
  }
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
  #ifdef DEBUG
    Serial.print("setPortion: ");
    Serial.print(start);
    Serial.print(" | ");
    Serial.println(finish);
  #endif
  fill_solid( &(leds[start]), finish, CRGB( r, g, b) );
  FastLED.show();
}

/**
 * Set a portion of LEDs with a delay between each LED
 * creates a slide effect
 * int r, g, b - color
 * int start - first LED
 * int finish - last led
 */
void setPortion(int r, int g, int b, int start, int finish, int wait) {
  for (int i = start; i < finish; i++) {
    leds[i].setRGB( r, g, b);
    FastLED.show();
    delay(wait);
  }
}

/**
 * Set the thermometer LEDs
 * int level - level to set
 *  0 to 5
 *  0 = off
 *  1-5 = thermo levels
 */
void setThermo(int level) {
  thermoLevel = level % 5;
  #ifdef DEBUG
    Serial.print("Set thermo level: ");
    Serial.println(level);
  #endif
  if (thermoLevel == 0) {
    // Set all thermo LEDs to off
    setPortion(0, 0, 0, 0, LED_START);
  } else {
    // Set thermo leds
    setPortion(THERMO_COLOR_R, THERMO_COLOR_G, THERMO_COLOR_B, THERMO_START, THERMO_LEVELS[level-1], THERMO_DELAY);
  }
}
