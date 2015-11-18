/* Robocourier/RoboPong
 *  by David Zong and Fergus Wong
 *  
 *  Robot: Arduino + ardumotor shield, QTC8-RC IR sensor, Zagros chassis + motors
 *  Cup: IR LED and IR sensor 
 *
 *  Game modes: continuous, around the world, random  
 */

//QTRSensors folder must be placed in your arduino libraries folder
#include <QTRSensors.h>  // Pololu QTR Library 
#include "pitches2.h"

//line sensor defines
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN  // emitter control pin not used.  If added, replace QTR_NO_EMITTER_PIN with pin#

// line sensor declarations
// sensors 0 through 7 are connected to digital pins 2 through 10, respectively (pin 3 is skipped and used by the Ardumoto controller)
// 0 is far Right sensor while 7 is far Left sensor
QTRSensorsRC qtrrc((unsigned char[]) {2, 4, 5, 6, 7, 8, 9,10},NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS]; // array with individual sensor reading values
unsigned int line_position=0; // value from 0-7000 to indicate position of line between sensor 0 - 7

// ArduMoto motor driver vars
// pwm_a/b sets speed.  Value range is 0-255.  For example, if you set the speed at 100 then 100/255 = 39% duty cycle = slow
// dir_a/b sets direction.  LOW is Forward, HIGH is Reverse 
int pwm_a = 3;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
int pwm_b = 11;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor)
int dir_a = 12;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
int dir_b = 13;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)

// motor tuning vars 
int calSpeed = 110;   // tune value motors will run while auto calibration sweeping turn over line (0-255)
int runspeed = 110;     // tune value motors will run while line-following

// Proportional Control loop vars
float error=0;
float PV =0 ;  // Process Variable value calculated to adjust speeds and keep on line
float kp = 0.7;  // This is the Proportional value. Tune this value to affect follow_line performance
int m1Speed=0; // (Left motor)
int m2Speed=0; // (Right motor)


// Ball sensor IR LED + SENSOR PINS
#define irLedPin 14          // A0; IR Led on this pin
#define irSensorPin 15       // A1; IR sensor on this pin

int irRead(int readPin, int triggerPin); //function prototype
int IRvalue;
int score = 0;

// Buzzer pin
int pin = 16;

void setup()
{
    winSound();
  
  //Set control pins to be outputs
  pinMode(pwm_a, OUTPUT);  
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);  
  pinMode(dir_b, OUTPUT);  

  //set both motors to stop
  analogWrite(pwm_a, 0);  
  analogWrite(pwm_b, 0);
    
  // delay to allow you to set the robot on the line, turn on the power, 
  // then move your hand away before it begins moving.
  delay(2000);

  // calibrate line sensor; determines min/max range of sensed values for the current course
  for (int i = 0; i <= 100; i++)  // begin calibration cycle to last about 2.5 seconds (100*25ms/call)
  {
    
    // auto calibration sweeping left/right, tune 'calSpeed' motor speed at declaration
    // just high enough all sensors are passed over the line. Not too fast.
    if (i==0 || i==60)  // slow sweeping turn right to pass sensors over line
    {
       digitalWrite(dir_a, LOW); 
       analogWrite(pwm_a, calSpeed);
       digitalWrite(dir_b, HIGH);  
       analogWrite(pwm_b, calSpeed);   
    }
    
    else if (i==20 || i==100) // slow sweeping turn left to pass sensors over line
    {
       digitalWrite(dir_a, HIGH); 
       analogWrite(pwm_a, calSpeed);
       digitalWrite(dir_b, LOW);  
       analogWrite(pwm_b, calSpeed);   
    }
    
    qtrrc.calibrate(); // reads all sensors with the define set 2500 microseconds (25 milliseconds) for sensor outputs to go low.
    
  }  // end calibration cycle

  // read calibrated sensor values and obtain a measure of the line position from 0 to 7000
  line_position = qtrrc.readLine(sensorValues);

  // read the value of only a single sensor to see the line.
  // when the value is greater than 200 the sensor sees the line.
  while (sensorValues[6] < 200)  // wait for line position to near center
  {
    line_position = qtrrc.readLine(sensorValues);
  }
   
  // find near center
  while (line_position > 4350) // continue loop until line position is near center
  {
    line_position = qtrrc.readLine(sensorValues);
  }
 
  // stop both motors
  analogWrite(pwm_b, 0); // stop right motor first which kinda helps avoid over run
  analogWrite(pwm_a, 0);

  // delay as indicator setup and calibration is complete
  delay(1000); 


  // IRSensor setup
  pinMode(irSensorPin, INPUT);
  pinMode(irLedPin, OUTPUT);
  Serial.begin(9600); 
  // prints title with ending line break 
  Serial.println("Program Starting"); 
  // wait for the long string to be sent 
  delay(100); 


  
} // end setup



void loop() // main loop
{
  // sense for stop points
  //stop_line(sensorValues);
    int stopcounter = 0;
  // check if all sensors see black. stop motors if true
  for (int x = 0; x<NUM_SENSORS; x++)
  {
    if (sensorValues[x] < 200){stopcounter = stopcounter + 1;}    
  } 
  if (stopcounter == 8)
  {
    m1Speed = 0;
    m2Speed = 0;
          digitalWrite(dir_a, LOW); 
          digitalWrite(dir_b, LOW);  
          analogWrite(pwm_a, m1Speed);
          analogWrite(pwm_b, m2Speed);
    delay(2000);
      digitalWrite(dir_a, LOW);  
      analogWrite(pwm_a, 120);
      digitalWrite(dir_b, LOW);  
      analogWrite(pwm_b, 120);
    delay(500);
    stopcounter = 0;
  }
  

//  // check if 0,1,6,7 sensors are black
//  if ( (sensorValues[0] > 200) && (sensorValues[1] > 700) && (sensorValues[6] > 700) && (sensorValues[7] > 700) )
//  {
//    m1Speed = 0;
//    m2Speed = 0;
//    delay(3000);
//   }
  
  // read calibrated sensor values and obtain a measure of the line position from 0 to 7000
  unsigned int line_position = qtrrc.readLine(sensorValues);
 
  // begin line following
  follow_line(line_position);

  // Detect goals ----------------------------------------------------------------------------------------
  IRvalue = irRead(irSensorPin, irLedPin);
  if (IRvalue ==  1){  // if goal scored, add it up
    score = score + 1;
    Serial.println("GOOOOOOOOL");
    Serial.println("score :");
    Serial.println(score);
//    coinSound();
    delay(1000);
    }
  Serial.println(IRvalue); //display the results
  

}  // end main loop

//
//void stop_line(unsigned int sensorValues)
//{
//  int stopcounter = 0;
//  // check if all sensors see black. stop motors if true
//  for (int x = 0; x<NUM_SENSORS; x++)
//  {
//    if (sensorValues[x] > 700){stopcounter = stopcounter + 1;}    
//  } 
//  if (stopcounter == 8)
//  {
//    m1speed = 0;
//    m2speed = 0;
//    delay(3000);
//  }
//} // end void stop_line   

// line following subroutine
//  Proportional Control Only
//  For a lesson in more advanced PID control, see the Zagros Tutorial on the Proportional-Derivative Control
//  http://www.instructables.com/id/Basic-PD-Proportional-Derivative-Line-Follower/

void follow_line(int line_position) //follow the line
{

  // 0 is far Right sensor while 7 (7000 return) is far Left sensor

  switch(line_position)
  {
       
    // Line has moved off the left edge of sensor
    // This will make it turn fast to the left
    case 7000:
           digitalWrite(dir_a, HIGH); 
           analogWrite(pwm_a, 200);
           digitalWrite(dir_b, LOW);  
           analogWrite(pwm_b, 200);
    break;

    // Line had moved off the right edge of sensor
    // This will make it turn fast to the right
    case 0:     
           digitalWrite(dir_a, LOW); 
           analogWrite(pwm_a, 200);
           digitalWrite(dir_b, HIGH);  
           analogWrite(pwm_b, 200);
    break;
 
    // The line is still within the sensors. 
    // This will calculate adjusting speed to keep the line in center.
    default:      
      error = (float)line_position - 3500; // 3500 is center measure of 7000 far left and 0 on far right
 
      // This sets the motor speed based on a proportional only formula.
      // kp is the floating-point proportional constant you need to tune. 
      // Maybe start with a kp value around 1.0, tuned in declared Proportional Control loop vars at the top of this code.
      // Note that it's very important you get your signs right, or else the
      // control loop will be unstable.
   
      // calculate the new Process Variable
      // this is the value that will be used to alter the speeds
      PV = kp * error;
  
      // this section limits the PV (motor speed pwm value)  
      // limit PV to 55
      if (PV > 55)
      {
        PV = 55;
      }
  
      if (PV < -55)
      {
        PV = -55;
      }
      
      // adjust motor speeds to correct the path
      // Note that if PV > 0 the robot needs to turn left
      m1Speed = runspeed - PV;
      m2Speed = runspeed + PV;
     
      //set motor speeds
      digitalWrite(dir_a, LOW);  
      analogWrite(pwm_a, m1Speed);
      digitalWrite(dir_b, LOW);  
      analogWrite(pwm_b, m2Speed);
      break;
  } 

} // end follow_line  

  
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


} // end irRead

void winSound() {
  // notes in the melody:
  int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_G4
  };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    8, 8, 8, 4, 4, 4, 8, 8, 2
  };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pin);
  }
}

void coinSound() {
  // coin sound
  tone(pin,NOTE_B5,100);
  delay(100);
  tone(pin,NOTE_E6,850);
  delay(800);
  noTone(pin);
}
