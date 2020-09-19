#include <mbed.h>
#include "step_listener.h"
#include "def_pins.h"
#include "stall_load_detector.h"

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

DigitalOut stall_(stall);
DigitalOut force_dir_(force_dir);
AnalogOut force_mag_(force_mag);

Timer t;
int speed;
int main(){
    t.start();
    StepListener driver(
        &step, &dir, &ms1, &ms2, &ms3,
        &stepIn, &dirIn, &ms1In, &ms2In, &ms3In
    ); 
    driver.readyToListen();
    Ammeter ammeter(&currentPin);
    StallLoadDetector detector(&ammeter, &stepper1);
    //speed = (int)driver.getCurrentSpeed();
    while(1){
        //speed = (int)driver.getCurrentSpeed();
        force_mag_ = detector.gettotalCurrent();
    //   s.printf("%d\r\n",(int)(force_mag*1000));
    }
    //test motor moving


}