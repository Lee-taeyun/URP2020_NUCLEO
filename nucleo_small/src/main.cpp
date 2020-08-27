#include "Stall_Load_Detection.h"

AccelStepper stepper1(AccelStepper::DRIVER, SET_SPEED, SET_DIR);

int data;
int speed = 0;
unsigned long last_time;
int dir = 0;
void setup()
{
  Serial.begin(9600);
  stepper1.moveTo(400);
  stepper1.setAcceleration(200);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(2000);
  //stepper1.setSpeed(0);

  pinMode(SET_MS1, OUTPUT);
  digitalWrite(SET_MS1,LOW);
  last_time = millis();
  
  dir=0;
  //Get_Linear_Regression3(stepper1);
  
}

void loop()
{
   stepper1.runSpeed();
  
  /*
  if (millis() - last_time > 1)
  {
    stepper1.runSpeed();
    if (dir == 0)
    {
      stepper1.runSpeed();
      speed = speed + 2;
      if (speed == 1000)
        dir = 1;
      stepper1.setSpeed(speed);
      stepper1.runSpeed();
    }
    else
    {
      stepper1.runSpeed();
      speed = speed - 2;
      if (speed == 0)
        dir = 0;
      stepper1.setSpeed(speed);
      stepper1.runSpeed();
    }
    last_time = millis();
  } //Serial.println(speed); //Serial.println(dir); stepper1.runSpeed(); //stepper1.runSpeed()
  stepper1.runSpeed();
  //Serial.println(speed);
*/
  
}