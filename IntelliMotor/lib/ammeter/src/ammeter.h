#ifndef AMMETER
#define AMMETER
#include "mbed.h"
#include "def_pins.h"
#include "filter.h"

#define LPF_alpha 0.99


class Ammeter{
    private:
        Filter filter; //LPF etc filter algorithm is implemented in Filter class
        AnalogIn current;
    public:
        Ammeter();
        Ammeter(PinName currentPin);
        double readCurrent();
        double readCurrentLPF();
};


#endif