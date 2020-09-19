#include "ammeter.h"
Ammeter::Ammeter():filter(){

}

Ammeter::Ammeter(AnalogIn* currentPin):filter(){
    this->current = currentPin;
}

double Ammeter::readCurrent(){
    return (double)(this->current->read());
}

double Ammeter::readCurrentLPF(){
    return this->filter.LPF(this->current->read(),LPF_alpha);
}