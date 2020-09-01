#include <mbed.h>
#include "def_pins.h"
#include "step_listener.h"
#include "stall_load_detector.h"
#include "Flash_handler.h"
static BufferedSerial pc(USBTX, USBRX);

//Timer t should be defined in main.cpp
//AccelStepper call Timer t as extern variable
Timer t;
AnalogIn currentPin(current_sense);
//DigitalOut intern_step(internal_step);
//DigitalOut intern_dir(internal_dir);

AccelStepper stepper1(AccelStepper::DRIVER,internal_step,internal_dir);

int main() {
  double speed=100;
  t.start();// must start timer in main
  pc.set_baud(9600);
  pc.set_format(
      /* bits */ 8,
      /* parity */ BufferedSerial::None,
      /* stop bit */ 1
  );
  
  int goal = 1200;
  stepper1.moveTo(goal);
  stepper1.setAcceleration(1000);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(2000);
  stepper1.setSpeed(400);

  
  Flash_handler Flash_handler;
  

  /*
  Ammeter ammeter(&currentPin);
  StallLoadDetector detector(&ammeter, &stepper1);
  detector.measureMotorCharacteristics();
  Flash_handler.Flash_erase();
  Flash_handler.Flash_write(detector.currentValues,NUM_OF_CURRENT_SAMPLE *sizeof(double));
  
  printf("\n\n");
  */
  double* array = new double[500];
  Flash_handler.Flash_read(array,NUM_OF_CURRENT_SAMPLE *sizeof(double));
  
  
  for(int i=0; i<500; i++){
    printf("%d\n", (int)(array[i]));
  }
  
  

  //Get_Linear_Regression2(stepper1);
  //AnalogIn 
  // put your setup code here, to run once:
  
  stepper1.setSpeed(400);
  while(1){
    stepper1.setSpeed(speed+200);
    speed=speed+0.0001;
    stepper1.runSpeed();
    //wait_us(10000);    // put your main code here, to run repeatedly:
  }
    return 0;
}