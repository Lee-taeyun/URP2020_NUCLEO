#include "stall_load_detector.h"

StallLoadDetector::StallLoadDetector(Ammeter* amm, AccelStepper* stepper){
    this->ammeter = amm;
    this->stepper = stepper;
}

void StallLoadDetector::measureMotorCharacteristics(){
    double* currentValues = new double[NUM_OF_CURRENT_SAMPLE];

    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    while(speed < MAX_STEP_SPEED){
        this->stepper->runSpeed();
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME){
            currentValues[idx] = ammeter->readCurrentLPF()*SAMPLE_VALUE_MULTIPLIER;
            idx++;
            speed = speed + skip_step;
            this->stepper->setSpeed(speed);
            last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        }
    }

    this->lineContainer = getTrendLine(currentValues, NUM_OF_CURRENT_SAMPLE , MAX_DEPTH_FOR_RECURSION);
    this->containerLen = sizeofLineContainer(this->lineContainer, 1<<MAX_DEPTH_FOR_RECURSION);
    delete[] currentValues;
}



void StallLoadDetector::measureMotorMeanCharacteristics(){
    double* currentValues = new double[NUM_OF_CURRENT_SAMPLE];

    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    double sampleMean = 0;
    unsigned int samplingCount = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    while(speed < MAX_STEP_SPEED){
        this->stepper->runSpeed();
        sampleMean += ammeter->readCurrentLPF()*SAMPLE_VALUE_MULTIPLIER;
        samplingCount++; // to divide sampleMean
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME ){
            currentValues[idx] = sampleMean/samplingCount;
            //start to sampleMean again 
            sampleMean = 0;
            samplingCount = 0;
            idx++;
            
            speed = speed + skip_step;
            this->stepper->setSpeed(speed);
            last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        }
    }

    this->lineContainer = getTrendLine(currentValues, NUM_OF_CURRENT_SAMPLE , MAX_DEPTH_FOR_RECURSION);
    this->containerLen = sizeofLineContainer(this->lineContainer, 1<<MAX_DEPTH_FOR_RECURSION);
    delete[] currentValues;
}

double StallLoadDetector::calculateCurrentFromSpeed(double speed){
    //Get current using array
    return getCurrentValue(speed, MAX_STEP_SPEED, NUM_OF_CURRENT_SAMPLE, this->lineContainer, this->containerLen);
}

StallLoadDetector::~StallLoadDetector(){
    free(this->lineContainer);    
}