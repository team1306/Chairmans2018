// Run a A4998 Stepstick from an Arduino UNO.
// Paul Hurley Aug 2015
int x; 
#define BAUD (9600)


void setup() 
{
  Serial.begin(BAUD);
  pinMode(3,OUTPUT); // Enable
  pinMode(5,OUTPUT); // Step
  pinMode(4,OUTPUT); // Dir
  digitalWrite(3,LOW); // Set Enable low
  digitalWrite(4, LOW);
}

void loop() 
{
  digitalWrite(3,LOW); // Set Enable low
//  digitalWrite(4,HIGH); // Set Dir high
  Serial.println("Loop 200 steps (1 rev)");
  for(x = 0; x < 32; x++) // Loop 200 times
  {
    digitalWrite(5,HIGH); // Output high
    delay(10); // Wait
    digitalWrite(5,LOW); // Output low
//    delay(100); // Wait
  }
  Serial.println("Pause");
  delay(1000); // pause one second
}
