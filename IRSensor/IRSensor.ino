/*
 * 20151109 some noise due to non-soldered wires contributing to false positive scores. should be fixed if wires permanently attached.
 */
//define pins
#define irLedPin 14          // A0; IR Led on this pin
#define irSensorPin 15       // A1; IR sensor on this pin

int irRead(int readPin, int triggerPin); //function prototype
int IRvalue;
int score = 0;

void setup()
{

  pinMode(irSensorPin, INPUT);
  pinMode(irLedPin, OUTPUT);
  Serial.begin(9600); 
  // prints title with ending line break 
  Serial.println("Program Starting"); 
  // wait for the long string to be sent 
  delay(100); 
}

void loop()
{  
  IRvalue = irRead(irSensorPin, irLedPin);
  if (IRvalue ==  1){  // if goal scored, add it up

    
    score = score + 1;
    Serial.println("GOOOOOOOOL");
    Serial.println("score :");
    Serial.println(score);
    delay(1000);
//    digitalWrite(13, HIGH);
//    delay(1000);
//    digitalWrite(13, LOW);
//    delay(100);
    }
  Serial.println(IRvalue); //display the results
  delay(10); //wait for the string to be sent
}

/******************************************************************************
 * This function can be used with a panasonic pna4602m ir sensor
 * it returns a zero if something is detected by the sensor, and a 1 otherwise
 * The function bit bangs a 38.5khZ waveform to an IR led connected to the
 * triggerPin for 1 millisecond, and then reads the IR sensor pin to see if
 * the reflected IR has been detected
 ******************************************************************************/
int irRead(int readPin, int triggerPin)
{
  int halfPeriod = 13; //one period at 38.5khZ is aproximately 26 microseconds
  int cycles = 38; //26 microseconds * 38 is more or less 1 millisecond
  int i;
  for (i=0; i <=cycles; i++)
  {
    digitalWrite(triggerPin, HIGH); 
    delayMicroseconds(halfPeriod);
    digitalWrite(triggerPin, LOW); 
    delayMicroseconds(halfPeriod - 1);     // - 1 to make up for digitaWrite overhead    
  }
    
  return digitalRead(readPin);


}
