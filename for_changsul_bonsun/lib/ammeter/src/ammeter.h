#ifndef AMMETER
#define AMMETER
#include "mbed.h"
#include "filter.h"

#define LPF_alpha 0.6


class Ammeter{
    private:
        Filter filter; //LPF etc filter algorithm is implemented in Filter class
        AnalogIn* current;
        //To prevent duplicated AnalogIn Current Pin, 
        //Get the pointer of pin not the object
    public:
        Ammeter();
        Ammeter(AnalogIn* currentPin);
        double readCurrent();
        double readCurrentLPF();
        void resetLPF();
};


#endif