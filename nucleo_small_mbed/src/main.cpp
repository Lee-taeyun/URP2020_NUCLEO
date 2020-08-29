#include <mbed.h>
#include "step_listener.h"
#include "AccelStepper.h"
#include "Stall_Load_Detection.h"

static BufferedSerial pc(USBTX, USBRX);

//Timer t should be defined in main.cpp
//AccelStepper call Timer t as extern variable
Timer t;
AccelStepper stepper1(AccelStepper::DRIVER,internal_step,internal_dir);


int main() {
  t.start();// must start timer in main
  pc.set_baud(9600);
  pc.set_format(
      /* bits */ 8,
      /* parity */ BufferedSerial::None,
      /* stop bit */ 1
  );
  stepper1.moveTo(400);
  stepper1.setAcceleration(2000);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(2000);
  stepper1.setSpeed(100);
  Get_Linear_Regression2(stepper1);
  //AnalogIn 
  // put your setup code here, to run once:

  while(1) {
    //stepper1.runSpeed();
    // put your main code here, to run repeatedly:

  }

}