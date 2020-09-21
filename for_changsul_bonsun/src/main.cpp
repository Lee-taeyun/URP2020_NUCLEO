#include <mbed.h>
#include "def_pins.h"
#include "step_listener.h"
#include "stall_load_detector.h"
#include "Flash_handler.h"
#include "mode_events.h"


//Timer t should be defined in main.cpp
//AccelStepper call Timer t as extern variable
Timer t;
AnalogIn currentPin(current_sense);

AccelStepper stepper1(AccelStepper::DRIVER, internal_step, internal_dir);

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

DigitalOut stall_(stall);
DigitalOut force_dir_(force_dir);
AnalogOut force_mag_(force_mag);


int main()
{
  int target = 800;
  StepListener driver( &step, &dir, &ms1, &ms2, &ms3, &stepIn, &dirIn, &ms1In, &ms2In, &ms3In);
  t.start(); // must start timer in main
  wait_us(1500000);
  stepper1.setAcceleration(2000);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(800);
  stepper1.setSpeed(500);
  stepper1.moveTo(target);
  Flash_handler Flash_handler; //Flash_memory handler
  Ammeter ammeter(&currentPin);
  StallLoadDetector detector(&ammeter, &stepper1);
  /*
  if (ms1 == 1)
  {
    detector.measureMotorMeanCharacteristics();
    Flash_handler.Flash_erase();
    Flash_handler.Flash_write(detector.currentValues, NUM_OF_CURRENT_SAMPLE * sizeof(double));
  }
  */
  Flash_handler.Flash_read(detector.currentValues, NUM_OF_CURRENT_SAMPLE * sizeof(double));

  int speed = 500;
  stepper1.setSpeed(speed);


  MODES mode = DEFAULT;
  unsigned int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
  while (1)
  { 
    mode = Determine_MODE();
    /// DEFAULT!!!
    if(mode == DEFAULT){
      if(!driver.isStop()){
        driver.stopToListen();
      }
      double mag = detector.AnalogOutForce(abs(speed), &force_mag_, &force_dir_);
      unsigned int current_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      if (mag > 0 && force_dir_ == 0 && current_time - last_time > 600){
        stepper1.setSpeed(-speed);
        speed = -speed;
        last_time = current_time;
      }else if(mag >0 && force_dir_ == 1 && current_time - last_time > 300){
        if(speed>0){
          stepper1.setSpeed(speed+100);
          speed = speed+100;
          if(speed ==1000){
            speed =500;
            stepper1.setSpeed(speed);
          }
            
        }else{
          stepper1.setSpeed(speed-100);
          speed = speed - 100;
          if(speed ==-1000)
            speed =-500;
            stepper1.setSpeed(speed);
        }
        last_time = current_time;
      }
      stepper1.runSpeed();

    //STEP_LISTENER
    }else if(mode == STEP_LISTENER){
      if(driver.isStop()){
        driver.readyToListen();
      }

    //MOTION_1
    }else if(mode == MOTION_1){
      if(!driver.isStop()){
        driver.stopToListen();
        stepper1.setCurrentPosition(0);
        stepper1.setAcceleration(2000);
      }
      
      stepper1.run();

      if(stepper1.distanceToGo() ==0){
        target = -0.8*target;
        stepper1.moveTo(target);
      }
      stepper1.run();
    }else if(mode == CALIBRATION){
    //rearrange the array
      detector.measureMotorMeanCharacteristics();
      Flash_handler.Flash_erase();
      Flash_handler.Flash_write(detector.currentValues, NUM_OF_CURRENT_SAMPLE * sizeof(double));
    
    }
  }
  return 0;
}

/*#include <mbed.h>
#include "def_pins.h"
#include "step_listener.h"
#include "stall_load_detector.h"
#include "Flash_handler.h"
#include "mode_events.h"

static BufferedSerial pc(USBTX, USBRX);

//Timer t should be defined in main.cpp
//AccelStepper call Timer t as extern variable
Timer t;
AnalogIn currentPin(current_sense);

AccelStepper stepper1(AccelStepper::DRIVER, internal_step, internal_dir);

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

DigitalOut stall_(stall);
DigitalOut force_dir_(force_dir);
AnalogOut force_mag_(force_mag);

//enum EVENTS{SAME_DIR, OP_DIR, SHORT_STROKE};
//enum MODES{DEFAULT,ALARM,STEP_LISTENER };

int main()
{

  //StepListener driver( &step, &dir, &ms1, &ms2, &ms3, &stepIn, &dirIn, &ms1In, &ms2In, &ms3In);
  t.start(); // must start timer in main
  pc.set_baud(9600);
  pc.set_format(
     //bits  //8,
     //parity  //BufferedSerial::None,
     //stop bit  //1);
/*
  stepper1.setAcceleration(2000);
  stepper1.setMinPulseWidth(20);
  stepper1.setMaxSpeed(800);
  stepper1.setSpeed(500);
  stepper1.moveTo(2400);
  Flash_handler Flash_handler; //Flash_memory handler
  Ammeter ammeter(&currentPin);
  StallLoadDetector detector(&ammeter, &stepper1);
  ms1.mode(PullDown);
  ms2.mode(PullDown);
  ms3.mode(PullDown);
  
  if (ms3 == 1)
  {
    detector.measureMotorMeanCharacteristics();
    Flash_handler.Flash_erase();
    Flash_handler.Flash_write(detector.currentValues, NUM_OF_CURRENT_SAMPLE * sizeof(double));
  }

  Flash_handler.Flash_read(detector.currentValues, NUM_OF_CURRENT_SAMPLE * sizeof(double));
  //driver.readyToListen();

  //Get_Linear_Regression2(stepper1);
  // put your setup code here, to run once:
  //double speed=200;
  int speed = 500;
  stepper1.setSpeed(speed);
  unsigned int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
  while (1)
  {
    speed = stepper1.speed();
    double mag = detector.AnalogOutForce(abs(speed), &force_mag_, &force_dir_);

    MODES MODE = Determine_MODE();
    EVENTS EVENT = Determine_EVENT(MODE);
    if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > 500)
    {
      if (EVENT == SAME_DIR){
        SAME_DIR_EVENT(&stepper1);
        last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      }
      else if (EVENT == OP_DIR){
        OP_DIR_EVENT(&stepper1);
        last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      }
      
    }

    
    stepper1.runSpeed();
  }
  return 0;
}*/