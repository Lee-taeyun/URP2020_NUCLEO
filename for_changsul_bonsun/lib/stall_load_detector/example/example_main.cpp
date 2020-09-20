#include <mbed.h>
#include "def_pins.h"
#include "AccelStepper.h"
#include "ammeter.h"
#include "stall_load_detector.h"

AnalogIn currentPin(current_sense);
Timer t;
AccelStepper stepper1(AccelStepper::DRIVER, internal_step, internal_dir);

int main(){
    Ammeter ammeter(&currentPin);
    StallLoadDetector detector(&ammeter, &stepper1);
    detector.measureMotorCharacteristics();
    //detector.measureMotorMeanCharacteristics();
    while(1){

    }
    return 0;
}