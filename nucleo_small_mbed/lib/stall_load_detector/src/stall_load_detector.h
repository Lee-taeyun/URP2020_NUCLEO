#ifndef STALL_LOAD_DETECTOR_H
#define STALL_LOAD_DETECTOR_H

#define DEBUG

#include "mbed.h"
#include "ammeter.h"
#include "AccelStepper.h"
#include "step_listener.h"


// NUM_OF_CURRENT_SAMPLE must be a divisor of MAX_STEP_SPEED
// MAX_STEP_SPEED = NUM_OF_CURRENT_SAMPLE * SKIP_STEP
#define NUM_OF_CURRENT_SAMPLE   500
#define MAX_STEP_SPEED          1000 //steps per second
#define SPEED_HOLDING_TIME          100   //ms
#define SAMPLE_VALUE_MULTIPLIER 100000
#define LOAD_CURRENT_SCALER     10    

class StallLoadDetector{
    private:
        Ammeter* ammeter;
        AccelStepper* stepper;
        Ticker runner;
        const static int skip_step = MAX_STEP_SPEED/NUM_OF_CURRENT_SAMPLE;
        //MAX_STEP_SPEED = NUM_OF_CURRENT_SAMPLE * skip_step

        Filter LPF_filter;//for GetLPFLoadCureent
        
    public:
        double *currentValues;
        StallLoadDetector(Ammeter* amm,AccelStepper* stepper1 );
        void measureMotorCharacteristics();
        void measureMotorMeanCharacteristics();
        double calculateCurrentFromSpeed(StepListener* steplistener);
        double getLoadCurrent(StepListener* steplistener);
        double gettotalCurrent(StepListener* steplistener);
        double calculateCurrentVarition(StepListener *steplistener)

        
        double getLPFLoadCurrent(StepListener* steplistener);
        ~StallLoadDetector();

        void AnalogOutForce(StepListener *steplistener,PwmOut *force_mag, DigitalOut *force_dir);
};
#endif