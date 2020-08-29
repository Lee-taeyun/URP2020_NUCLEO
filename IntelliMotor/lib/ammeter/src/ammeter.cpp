#include "ammeter.h"

Ammeter::Ammeter():current(current_sense), filter(){

}

Ammeter::Ammeter(PinName currentPin):current(currentPin), filter(){

}

double Ammeter::readCurrent(){
    return (double)(this->current.read());
}

double Ammeter::readCurrentLPF(){
    return this->filter.LPF(this->current.read(),LPF_alpha);
}