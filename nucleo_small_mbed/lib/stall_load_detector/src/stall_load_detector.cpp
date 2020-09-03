#include "stall_load_detector.h"

StallLoadDetector::StallLoadDetector(Ammeter* amm, AccelStepper* stepper1){
    this->stepper = stepper1;
    this->ammeter = amm;
    this->currentValues = new double[NUM_OF_CURRENT_SAMPLE];
}
StallLoadDetector::~StallLoadDetector(){
    delete[] this->currentValues;    
}
/*
void StallLoadDetector::startToRun(){
    //Meausre stepSpeed per tickDuration
    this->runner.attach([this](void)->void{
        this->stepper->runSpeed();
    }, 0.00001);
}
*/

void StallLoadDetector::measureMotorCharacteristics(){
    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    //this->startToRun();
    while(speed < MAX_STEP_SPEED){
        this->stepper->runSpeed();
        ammeter->readCurrentLPF();//we should readCurrentLPF() ALWAYS, NOT only just write down
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME){
            this->stepper->runSpeed();
            this->currentValues[idx] = (ammeter->readCurrentLPF())*SAMPLE_VALUE_MULTIPLIER;
            idx++;
            speed = speed + skip_step;
            
            this->stepper->setSpeed(speed);
            this->stepper->runSpeed();
            last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        }
        //ammeter->readCurrentLPF();
    }

    //#ifdef DEBUG
    /*
    for(int i = 0; i < NUM_OF_CURRENT_SAMPLE; i++){
        printf("%d\n",(int)(this->currentValues[i]));
    }
    */
    //#endif
}

void StallLoadDetector::measureMotorMeanCharacteristics(){
    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    long double sampleMean = 0;
    unsigned int samplingCount = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    //this->startToRun();
    while(speed < MAX_STEP_SPEED){
       this->stepper->runSpeed();
        sampleMean += ammeter->readCurrentLPF()*SAMPLE_VALUE_MULTIPLIER;
        samplingCount++; // to divide sampleMean
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME ){
            this->currentValues[idx] = (sampleMean)/samplingCount;
            this->stepper->runSpeed();
            //start to sampleMean again 
            sampleMean = 0;
            samplingCount = 0;
            idx++;
            
            speed = speed + skip_step;
            this->stepper->setSpeed(speed);
            this->stepper->runSpeed();
            last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
        }
    }

    //#ifdef DEBUG
    //for(int i = 0; i < NUM_OF_CURRENT_SAMPLE; i++){
    //    int value = (int)(this->currentValues[i]);
    //    printf("%d\n",(int)(value));
    //}
    //#endif
}

double StallLoadDetector::calculateCurrentFromSpeed(StepListener* steplistener){
    //Get current using array
    int speed = (int)(steplistener->getCurrentSpeed());
    if( speed >1000 || speed<0) speed = speed %1000;
    int quotient = ((int)speed)/skip_step;
    int remainder = ((int)speed)%skip_step;
    if(remainder){
        return (this->currentValues[quotient]+this->currentValues[quotient+remainder])/2;
    }else{
        return this->currentValues[quotient];
    }
}

double StallLoadDetector::gettotalCurrent(StepListener* steplistener){
    return SAMPLE_VALUE_MULTIPLIER*(ammeter->readCurrentLPF());
}
double StallLoadDetector::getLoadCurrent(StepListener* steplistener){
    return SAMPLE_VALUE_MULTIPLIER*(ammeter->readCurrentLPF()) - calculateCurrentFromSpeed(steplistener);
}

double StallLoadDetector::getLPFLoadCurrent(StepListener* steplistener){
    double LoadCurrent = getLoadCurrent(steplistener);
    
    return LoadCurrent - LPF_filter.LPF(LoadCurrent,LPF_alpha);
}