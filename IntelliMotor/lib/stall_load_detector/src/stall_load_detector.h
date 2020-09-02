#ifndef STALL_LOAD_DETECTOR_H
#define STALL_LOAD_DETECTOR_H

#include "mbed.h"
#include "ammeter.h"
#include "AccelStepper.h"
#include "regression.h"

// NUM_OF_CURRENT_SAMPLE must be a divisor of MAX_STEP_SPEED
// MAX_STEP_SPEED = NUM_OF_CURRENT_SAMPLE * SKIP_STEP
#define NUM_OF_CURRENT_SAMPLE   500
#define MAX_STEP_SPEED          1000 //steps per second
#define SPEED_HOLDING_TIME      10   //ms
#define SAMPLE_VALUE_MULTIPLIER 1000

#define MAX_DEPTH_FOR_RECURSION 5

class StallLoadDetector{
    private:
        Ammeter* ammeter;
        AccelStepper* stepper;
        const static int skip_step = MAX_STEP_SPEED/NUM_OF_CURRENT_SAMPLE;
        //MAX_STEP_SPEED = NUM_OF_CURRENT_SAMPLE * skip_step
    public:
        Line* lineContainer;
        int containerLen;
        StallLoadDetector(Ammeter* amm, AccelStepper* stepper);
        ~StallLoadDetector();
        void measureMotorCharacteristics();
        void measureMotorMeanCharacteristics();
        double calculateCurrentFromSpeed(double speed);
        //now support any speed between 0 - MAX_STEP_SPEED to calculate current value
        
};
#endif