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

AccelStepper stepper1(AccelStepper::DRIVER,internal_step,internal_dir);

InterruptIn step(external_step);
InterruptIn dir(external_dir);
InterruptIn ms1(external_ms1);
InterruptIn ms2(external_ms2);
InterruptIn ms3(external_ms3);

DigitalOut stepIn(internal_step);
DigitalOut dirIn(internal_dir);
DigitalOut ms1In(internal_ms1);
DigitalOut ms2In(internal_ms2);
DigitalOut ms3In(internal_ms1);


int main() {

  StepListener driver( &step, &dir, &ms1, &ms2, &ms3, &stepIn, &dirIn, &ms1In, &ms2In, &ms3In); 
  
  driver.readyToListen();
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

  
  Flash_handler Flash_handler;//Flash_memory handler
  Ammeter ammeter(&currentPin);
  StallLoadDetector detector(&ammeter, &stepper1);
  
  
  if(ms3 ==1){
    detector.measureMotorMeanCharacteristics();
    Flash_handler.Flash_erase();
    Flash_handler.Flash_write(detector.currentValues,NUM_OF_CURRENT_SAMPLE *sizeof(double));
  }
  Flash_handler.Flash_read(detector.currentValues,NUM_OF_CURRENT_SAMPLE *sizeof(double));
  
  //Get_Linear_Regression2(stepper1);
  // put your setup code here, to run once:
  //double speed=200;
  stepper1.setSpeed(400);

  while(1){
  printf("%ld\n",(long)(detector.getLPFLoadCurrent(&driver)));
  //printf("%d\n",(int)(detector.calculateCurrentFromSpeed(&driver))); 
  //printf("%d\n",(int)(detector.gettotalCurrent(&driver)));
  //printf("%d\n",(int)(driver.getCurrentSpeed()));  
  //printf("%d\n",(int)(driver.returnSpeed()));
  // put your main code here, to run repeatedly:
  }
    return 0;
}