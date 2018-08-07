#include <FastLED.h>
#include <Wire.h>
// How many leds in your strip?
#define NUM_LEDS 235 
#define DATA_PIN 12
#define FORWARD 0
#define BACKWARD 1
#define SLOW 250
#define MEDIUM 50
#define FAST 5

const int HEX_LEDS_START = 211;  // TODO: figure this out
const int HEX_LED_R = 255;
const int HEX_LED_G = 255;
const int HEX_LED_B = 255;

// Thermometer
const int THERMO_START = 0; // First thermo led
const int THERMO_END=31;// Last thermo led
CRGB leds[NUM_LEDS];

boolean direction = FORWARD;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() { 
  for(int i=0;i<255;i++){
    allColor(Wheel(i));
    delay(10);
   }
    for(int i=0;i<255;i++){
    allColor(Wheel(254-i));
    delay(10);
   }
  
  CRGB colors[5];
  for(int i=0;i<5;i++){
    colors[i]=randomColor();
  }
  explosion(colors,5,FAST,false);
  for(int i=0;i<5;i++){
    colors[i]=randomColor();
  }
  explosion(colors,5,FAST,false);
  for(int i=0;i<5;i++){
    colors[i]=randomColor();
  }
  explosion(colors,5,FAST,false);

  cylon(randomColor(),10,FAST);
  cylon(randomColor(),10,FAST);

  for(int i=0;i<3;i++){
    CRGB c=randomColor();
    colorWipe(c,FAST,FORWARD);
    colorWipe(CRGB::Black,FAST,FORWARD);
  }
   for(int i=0;i<3;i++){
    CRGB c=randomColor();
    colorWipe(c,FAST,BACKWARD);
    colorWipe(CRGB::Black,FAST,BACKWARD);
  }
  rainbow(3,FAST,FORWARD);
   theaterChaseRainbow(1,  MEDIUM);
}

// Changes all LEDS to given color
void allColor(CRGB c){
  for(int i=0; i<NUM_LEDS; i++){
    setLEDs(i,c);
  }
  FastLED.show();
}

void allRandom(){
  for(int i=0; i<NUM_LEDS; i++){
   setLEDs(i, randomColor());
  }
  FastLED.show(); 
}

// Random disolve colors
void disolve(int simultaneous, int cycles, int speed){
  for(int i=0; i<cycles; i++){
    for(int j=0; j<simultaneous; j++){
      int idx = random(NUM_LEDS);
      setLEDs(idx,CRGB::Black);
    }
    FastLED.show();
    delay(speed);
  }

  allColor(CRGB::Black);
}

// Flashes given color
// If c==NULL, random color flash
void flash(CRGB c, int count, int speed){
  for(int i=0; i<count; i++){
    if(c){
      allColor(c);
    }
    else{
      allColor(randomColor());
    }
    delay(speed);
    allColor(CRGB::Black);
    delay(speed);
  }
}

// Wipes color from end to end
void colorWipe(CRGB c, int speed, int direction){
  for(int i=0; i<NUM_LEDS; i++){
    if(direction == FORWARD){
      setLEDs(i,c);
    }
    else{
      setLEDs(NUM_LEDS-1-i, c);
    }
    FastLED.show();
    delay(speed);
  }
}

// Rainbow colors that slowly cycle across LEDs
void rainbow(int cycles, int speed,int direction){ // TODO direction
    for(int j=0; j<256*cycles; j++) {
      for(int i=0; i< NUM_LEDS; i++) {
        if(direction==FORWARD){
          setLEDs(i, Wheel(((i * 256 / NUM_LEDS) + j) % 255));}
        else{
          setLEDs(i,Wheel(((i * 256 / NUM_LEDS) + j) % 255));}   
      }
      FastLED.show();
      delay(speed);
    }
}

// Theater-style crawling lights
void theaterChase(CRGB c, int cycles, int speed){ // TODO direction

  for (int j=0; j<cycles; j++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        setLEDs(pos, c);    //turn every third pixel on
      }
      FastLED.show();

      delay(speed);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        setLEDs(i+q,CRGB::Black);        //turn every third pixel off
      }
    }
  }
}

// Theater-style crawling lights with rainbow effect
void theaterChaseRainbow(int cycles, int speed){ // TODO direction, duration
  for (int j=0; j < 256 * cycles/3; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        byte wheel=-3*i-3*j;
        while(wheel<0){
          wheel=wheel+255;
        }
        setLEDs(pos,Wheel(wheel % 255));    //turn every third pixel on
      }
      FastLED.show();

      delay(speed);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        setLEDs(i+q, CRGB::Black);  //turn every third pixel off
      }
    }
  }
}

// Random flashes of lightning
void lightning(CRGB c, int simultaneous, int cycles, int speed){
  int flashes[simultaneous];

  for(int i=0; i<cycles; i++){
    for(int j=0; j<simultaneous; j++){
      int idx = random(NUM_LEDS);
      flashes[j] = idx;
      setLEDs(idx,c ? c : randomColor());
    }
    FastLED.show();
    delay(speed);
    for(int s=0; s<simultaneous; s++){
      setLEDs(flashes[s], CRGB::Black);
    }
    delay(speed);
  }
}

// Sliding bar across LEDs
void cylon(CRGB c, int width, int speed){
  // First slide the leds in one direction
  for(int i = 0; i <= NUM_LEDS-width; i++) {
    for(int j=0; j<width; j++){
      setLEDs(i+j, c);
    }

    FastLED.show();

    // now that we've shown the leds, reset to black for next loop
    for(int j=0; j<5; j++){
      setLEDs(i+j, CRGB::Black);
    }
    delay(speed);
  }

  // Now go in the other direction.  
  for(int i = NUM_LEDS-width; i >= 0; i--) {
    for(int j=0; j<width; j++){
      setLEDs(i+j, c);
    }
    FastLED.show();
    for(int j=0; j<width; j++){
      setLEDs(i+j,CRGB::Black);
    }

    delay(speed);
  }
}

// Display alternating stripes
void stripes(CRGB c1, CRGB c2, int width){
  for(int i=0; i<NUM_LEDS; i++){
    if(i % (width * 2) < width){
      setLEDs(i,c1);
    }
    else{
      setLEDs(i, c2);
    } 
  }
  FastLED.show();
}

// Theater-style crawling of stripes
void stripesChase(CRGB c1, CRGB c2, int width, int cycles, int speed){ // TODO direction

}
void explosion(CRGB colors[],int arr_length, int speed,boolean fade){
  for(int j=0;j<(NUM_LEDS)/2+2;j++){
  for(int i=0;i<j;i++){
    CRGB c=colors[((j-i)/10)%arr_length];
     setLEDs(NUM_LEDS/2+i,c);
     setLEDs(NUM_LEDS/2-i,c);
   
  }
  FastLED.show();
  delay(speed);
  }
  if(fade){//if fade set all leds to black using a sliding blackout
   for(int j=0;j<(NUM_LEDS)/2+2;j++){
  for(int i=0;i<j;i++){
    CRGB c=CRGB::Black;
      setLEDs(NUM_LEDS/2+i,c);
     setLEDs(NUM_LEDS/2-i,c);
  }
  FastLED.show();
  delay(speed);
  }
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(1*(WheelPos * 3)/4, 1*(255 - WheelPos * 3)/4, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(1*(255 - WheelPos * 3)/4, 0, 1*(WheelPos * 3)/4);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, 1*(WheelPos * 3)/4, 1*(255 - WheelPos * 3)/4);
  }
}

CRGB randomColor(){
  return Wheel(random(256)); 
}
void setLEDs(int index,CRGB value){
  index=(122+index)%NUM_LEDS;
  if(index+1>THERMO_START&&index<THERMO_END){
    //if it would be written to the thermometor, dont
  }else if(index+1>HEX_LEDS_START-(THERMO_END-THERMO_START)&&index<HEX_LEDS_START){
    //if it would be written to the leds behind the thermometer ,write it to the thermometer and its original location
    leds[index]=value;
    leds[HEX_LEDS_START-index]=value;
  }else{
      leds[index]=value;
  }
}
