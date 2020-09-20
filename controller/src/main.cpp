#include <Arduino.h>
#include <AccelStepper.h>

#define BTN 21

#define STEP_PIN 17
#define DIR_PIN 5
#define MS1 18
#define MS2 19

unsigned long lastTime = 0;
unsigned int mode = 0;
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void sendMode(int mode){
  digitalWrite(MS1, mode&0x01);
  digitalWrite(MS2, (mode>>1)&0x01);
}

void setMode(){
  unsigned long currentTime = millis();
  if(currentTime-lastTime >= 500){
    mode = (mode + 1)%4;
  }
  lastTime = currentTime;
  sendMode(mode);
}

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(BTN, setMode, RISING);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(mode);
  switch (mode)
  {
  case 0:
    /* code */
    break;
  case 1:
    break;
  case 2:
    break;
  case 3:
    break;
  default:
    break;
  }

}