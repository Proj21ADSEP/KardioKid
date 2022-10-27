/*
  Exercise 360 degree Servo motor
  Simply runs servo in one direction for 3 seconds, stops, then reverses direction
  Uses built-in Servo.h library
*/
#include "Servo.h"
#define SERVO_PIN 9   // Can use any PWM pin
int stopValue = 1500;
int stepValue = 100;
int limitSwitch = 0;
int stopPosition = 0;


Servo servo;          // creates servo object used to control the servo motor
//===============================================================================
//  Initialization
//===============================================================================
void setup()
{
  delay(4000);
  pinMode(limitSwitch, INPUT_PULLUP);
  
  SerialUSB.begin(9600);
  servo.attach(SERVO_PIN); // assigns PWM pin to the servo object
  servo.writeMicroseconds(stopValue);  // Stop
  delay(50);

  checkOnStartup();
  
}
//===============================================================================
//  Main
//===============================================================================
void loop()
{
   

  if (SerialUSB.available() > 0) {
  // Check for data in the serial port; if none go do something else
  
  int serialCommand = SerialUSB.read();
  
  if (serialCommand == 48){
    // call moveUp function
    // this CLOSES the valve; UP is away from the body of the valve
    moveUp();
    //SerialUSB.println(serialCommand);
    SerialUSB.println(String("Moving Up...      "));
    delay(5);
    }

  else if (serialCommand == 49) {
    // call moveDown function
    // this OPENS the valve; DOWN is towards the body of the valve
    moveDown();
    //SerialUSB.println(serialCommand);
    SerialUSB.println(String("Moving Down...    "));
    delay(5);
    }

  else if (serialCommand == 50) {
    // increment stop value
    ++stopValue;
    //SerialUSB.println(serialCommand);
    SerialUSB.println(String("Center Value is ") + stopValue);
    delay(10);
    }

  else if (serialCommand == 51) {
    // decrement stop value
    --stopValue;
    //SerialUSB.println(serialCommand);
    SerialUSB.println(String("Center Value is ") + stopValue);
    delay(10);
    }

  else if (serialCommand == 52) {
    // increment step value by 5
    stepValue = stepValue + 5;
    //SerialUSB.println(serialCommand);
    SerialUSB.println(String("Step Value is ") + stepValue);
    delay(10);
    }

  else if (serialCommand == 53) {
    // decrement step value by 5; minimum is 5
    stepValue = stepValue - 5;
    if (stepValue == 0) {
      stepValue = 5;
    }
    //SerialUSB.println(serialCommand);
    SerialUSB.println(String("Step Value is ") + stepValue);
    delay(10);
    }

   else if (serialCommand == 54) {
    SerialUSB.println("Centering Zero Position");
    delay(10);
    checkOnStartup();
   }

  else {
    // ignore command
    }
  }

//SerialUSB.println("Idle..............");
servo.writeMicroseconds(stopValue);  // Always be in stop when Idle

delay(100);

}

void moveDown() {

  
    // Always assume OK to move the valve up
    servo.writeMicroseconds(stopValue-stepValue); //Spin clockwise
    delay(250);
    servo.writeMicroseconds(stopValue); // Stop
    delay(5);
    
  }

void moveUp() {
  
  if(digitalRead(limitSwitch) == HIGH){
    // move down further if not at stop position
    servo.writeMicroseconds(stopValue+stepValue);  // Spin counterclockwise
    delay(250);
    servo.writeMicroseconds(stopValue);  // Stop
    delay(5);
    }
}

void checkOnStartup() {

  while (digitalRead(limitSwitch) == HIGH){ // HIGH means stop hasn't been reached
    moveUp();
    }

  // move up one tick and check for still off or not; if not, then center may not be at middle
  while (digitalRead(limitSwitch) == LOW){
    servo.writeMicroseconds(stopValue-stepValue); //Spin clockwise
    delay(250);
    servo.writeMicroseconds(stopValue); // Stop
    delay(5);
    --stopValue; // keep ticking this down until the original step of 100 gets the servo body off the limit switch
    }

  moveUp();
  if (digitalRead(limitSwitch) == LOW){
    // do nothing, that step closed the switch again
  }
  else {
    stopValue = stopValue + 10; // tick up slightly
    }

  servo.writeMicroseconds(stopValue-(stepValue*5)); //Spin clockwise to open to open position
  delay(5750);
  servo.writeMicroseconds(stopValue); // Stop
  delay(5);
  SerialUSB.println(String("Center value is ") + stopValue);  
}
