#include <mbed.h>
#include "step_listener.h"

int main(){
    StepListener driver; //If no arguments, Pins defined in def_pins.h will be allocated automatically
    driver.readyToListen();
    while(1){

    }
}