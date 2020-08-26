#include <Arduino.h>
#include "Stall_Load_Detection.h"


AccelStepper stepper1(AccelStepper::DRIVER, SET_SPEED, SET_DIR);


int data;
int speed=0;
int last_time;
int dir=0;
void setup() {
  Serial.begin(9600);
  stepper1.moveTo(400);
  stepper1.setAcceleration(6400);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(1000);
  stepper1.setSpeed(60);
  speed=0;
  last_time = millis();
  dir=0;
  Get_Linear_Regression(stepper1);
}





void loop() {
  stepper1.runSpeed();
}