#include "stall_load_detector.h"

StallLoadDetector::StallLoadDetector(Ammeter* amm, AccelStepper* stepper){
    this->ammeter = amm;
    this->stepper = stepper;
}

void StallLoadDetector::measureMotorCharacteristics(){
    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    while(speed < MAX_STEP_SPEED){
        this->stepper->runSpeed();
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME){
            this->currentValues[idx] = ammeter->readCurrentLPF()*SAMPLE_VALUE_MULTIPLIER;
            idx++;
            speed = speed + skip_step;
            this->stepper->setSpeed(speed);
            last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        }
    }

    #ifdef DEBUG
    for(int i = 0; i < NUM_OF_CURRENT_SAMPLE; i++){
        printf("%d\n",(int)(this->currentValues[i]));
    }
    #endif
}

void StallLoadDetector::measureMotorMeanCharacteristics(){
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
            this->currentValues[idx] = sampleMean/samplingCount;
            //start to sampleMean again 
            sampleMean = 0;
            samplingCount = 0;
            idx++;
            
            speed = speed + skip_step;
            this->stepper->setSpeed(speed);
            last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        }
    }

    #ifdef DEBUG
    for(int i = 0; i < NUM_OF_CURRENT_SAMPLE; i++){
        printf("%d\n",(int)(this->currentValues[i]));
    }
    #endif
}

double StallLoadDetector::calculateCurrentFromSpeed(int speed){
    //Get current using array
    int quotient = speed/skip_step;
    int remainder = speed%skip_step;
    if(remainder){
        return (this->currentValues[quotient]+this->currentValues[quotient])/2;
    }else{
        return this->currentValues[quotient];
    }
}