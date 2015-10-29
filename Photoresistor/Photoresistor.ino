int ledPin = 11;
int photoresistor = 0; // A0 pin
int scoreledPin = 13;

int lightreading;
int prevlightreading;
int delta_lightreading;
int delta_threshold = 40; // change in photoresistor value corresponding to ball interrupting light path
 
void setup()  {
  pinMode(ledPin, OUTPUT);
  pinMode(scoreledPin,OUTPUT);
  
  Serial.begin(9600);
  prevlightreading = analogRead(photoresistor);
  lightreading = analogRead(photoresistor);
}
 
 
void loop()  {
  digitalWrite(ledPin, HIGH);
  lightreading = analogRead(photoresistor); // value of photoresistor
//  Serial.print("current reading ");
//  Serial.println(lightreading);
//
//  Serial.print("prev reading ");
//  Serial.println(prevlightreading);
  
  delta_lightreading = lightreading - prevlightreading;
  Serial.print("delta ");
  Serial.println(delta_lightreading);
  
  // The delay can be change to get the desired dimming effect
  delay(20);                            

  // detect change in lightreading (ball passed through)
  if (delta_lightreading > delta_threshold)
  {
    // count score
    Serial.println("GOAL GOAL GOAL GOAL GOAL GOAL GOAL GOAL GOAL");
    digitalWrite(scoreledPin,HIGH);
    delay(1000);
    digitalWrite(scoreledPin,LOW);
    delay(1000);
  }
  prevlightreading = lightreading;
}
