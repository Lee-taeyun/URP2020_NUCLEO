#ifndef STEP_PASS
#define STEP_PASs
#include "mbed.h"

#define EXTERNAL_STEP_PIN PB_8
#define EXTERNAL_DIR_PIN PB_6
#define EXTERNAL_MS1 PB_1
#define EXTERNAL_MS2 PB_0
#define EXTERNAL_MS3 PC_5

#define INTERNAL_STEP_PIN PA_5
#define INTERNAL_DIR_PIN PA_4
#define INTERNAL_MS1 PC_4
#define INTERNAL_MS2 PA_7
#define INTERNAL_MS3 PA_6

#define NUM_OF_MS_PINS 3

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

        void _stepUp();
        void _stepDown();
        void _dirUp();
        void _dirDown();
        void _ms1Up();
        void _ms1Down();
        void _ms2Up();
        void _ms2Down();
        void _ms3Up();
        void _ms3Down();

    public:
        StepListener(PinName step = EXTERNAL_STEP_PIN, PinName dir = EXTERNAL_DIR_PIN, 
                    PinName ms1 = EXTERNAL_MS1, PinName ms2 = EXTERNAL_MS2, PinName ms3 = EXTERNAL_MS3,
                    PinName stepIn = INTERNAL_STEP_PIN, PinName dirIn = INTERNAL_DIR_PIN,
                    PinName ms1In = INTERNAL_MS1, PinName ms2In = INTERNAL_MS2, PinName ms3In = INTERNAL_MS3);
};

#endif 




