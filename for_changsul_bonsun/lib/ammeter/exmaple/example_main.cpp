#include <mbed.h>
#include "def_pins.h"
#include "ammeter.h"

AnalogIn currentPin(current_sense);

int main(){
    Ammeter ammeter(&currentPin);

    while(1){
        ammeter.readCurrent();
        ammeter.readCurrentLPF();
    }
    return 0;
}