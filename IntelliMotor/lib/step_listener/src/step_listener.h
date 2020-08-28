#ifndef STEP_LISTENER
#define STEP_LISTENER
#include "mbed.h"
#include "def_pins.h"

class StepListener{
    private:
        InterruptIn step;
        InterruptIn dir;
        InterruptIn ms1;
        InterruptIn ms2;
        InterruptIn ms3;

        DigitalOut stepIn;
        DigitalOut dirIn;
        DigitalOut ms1In;
        DigitalOut ms2In;
        DigitalOut ms3In;


    public:
        StepListener();
        StepListener(PinName step, PinName dir, 
                    PinName ms1, PinName ms2, PinName ms3,
                    PinName stepIn, PinName dirIn,
                    PinName ms1In, PinName ms2In, PinName ms3In);

        void readyToListen();
};

#endif 




