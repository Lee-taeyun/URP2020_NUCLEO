#ifndef STEP_LISTENER
#define STEP_LISTENER

#include "mbed.h"
#include "def_pins.h"

#define DEFAULT_TICK_DURATION 1 // 1 second

class StepListener{
    private:
        InterruptIn* step;
        InterruptIn* dir;
        InterruptIn* ms1;
        InterruptIn* ms2;
        InterruptIn* ms3;

        DigitalOut* stepIn;
        DigitalOut* dirIn;
        DigitalOut* ms1In;
        DigitalOut* ms2In;
        DigitalOut* ms3In;

        /*
        static const int tickDuration = DEFAULT_TICK_DURATION;
        unsigned int stepTicks = 0;
        unsigned int lastStepTicksPerDuration = 0;
        Ticker speedometer;
        
        */

        //below is taeyun's code
        double speed;
        unsigned long last_time;
        unsigned long duration;

    public:
        
        StepListener(InterruptIn* step, InterruptIn* dir, 
                    InterruptIn* ms1, InterruptIn* ms2, InterruptIn* ms3,
                    DigitalOut* stepIn, DigitalOut* dirIn,
                    DigitalOut* ms1In, DigitalOut* ms2In, DigitalOut* ms3In);

        void readyToListen();
        //double currentSpeedStepsPerSeconds();

        void calculateSpeed();
        double getCurrentSpeed();
};

#endif 




