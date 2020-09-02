#include <mbed.h>
#include "step_listener.h"
#include "def_pins.h"

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

int main(){
    StepListener driver(
        &step, &dir, &ms1, &ms2, &ms3,
        &stepIn, &dirIn, &ms1In, &ms2In, &ms3In
    ); 
    driver.readyToListen();
    while(1){

    }
}