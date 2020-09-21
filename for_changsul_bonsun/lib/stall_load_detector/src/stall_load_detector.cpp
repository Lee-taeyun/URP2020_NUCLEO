#include "stall_load_detector.h"

StallLoadDetector::StallLoadDetector(Ammeter *amm, AccelStepper *stepper1)
{
    this->stepper = stepper1;
    this->ammeter = amm;
    this->currentValues = new double[NUM_OF_CURRENT_SAMPLE];
}
StallLoadDetector::~StallLoadDetector()
{
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

void StallLoadDetector::measureMotorCharacteristics()
{
    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    //this->startToRun();
    while (speed < MAX_STEP_SPEED)
    {
        this->stepper->runSpeed();
        ammeter->readCurrentLPF(); //we should readCurrentLPF() ALWAYS, NOT only just write down
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME)
        {
            this->stepper->runSpeed();
            this->currentValues[idx] = (ammeter->readCurrentLPF());
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

void StallLoadDetector::measureMotorMeanCharacteristics()
{
    extern Timer t;
    unsigned int speed = 0;
    unsigned int idx = 0;
    long double sampleMean = 0;
    //long double sampleSquareMean=0;
    unsigned int samplingCount = 0;
    int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
    this->stepper->setSpeed(speed);
    //this->startToRun();
    while (speed < MAX_STEP_SPEED)
    {
        this->stepper->runSpeed();
        double temp = ammeter->readCurrentLPF();
        sampleMean += temp;
        //sampleSquareMean += temp*temp;
        samplingCount++; // to divide sampleMean
        if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > SPEED_HOLDING_TIME)
        {
            this->currentValues[idx] = (sampleMean) / samplingCount;
            //this->currentValues[idx+NUM_OF_CURRENT_SAMPLE] = sampleSquareMean / samplingCount-(this->currentValues[idx]*this->currentValues[idx]);
            //variation
            this->stepper->runSpeed();
            //start to sampleMean again
            sampleMean = 0;
            //sampleSquareMean =0;
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

double StallLoadDetector::calculateCurrentFromSpeed(double speed_)
{
    //Get current using array
    int speed = (int)(speed_);
    if (speed > 1000 || speed < 0)
        speed = speed % 1000;
    int quotient = ((int)speed) / skip_step;
    int remainder = ((int)speed) % skip_step;
    if (remainder)
    {
        return (this->currentValues[quotient] + this->currentValues[quotient + 1]) / 2;
    }
    else
    {
        return this->currentValues[quotient];
    }
}


double StallLoadDetector::gettotalCurrent()
{
    return (ammeter->readCurrentLPF());
}
double StallLoadDetector::getLoadCurrent(double speed_)
{
    return ammeter->readCurrentLPF() / calculateCurrentFromSpeed(speed_)-1;
}

double StallLoadDetector::getLPFLoadCurrent(double speed_)
{
    double LoadCurrent = getLoadCurrent(speed_);
    
    return LoadCurrent - LPF_filter.LPF(LoadCurrent, LPF_alpha);
}
double StallLoadDetector::AnalogOutForce(double speed_,AnalogOut *force_mag, DigitalOut *force_dir)
{
    double LoadCurrent = getLoadCurrent(speed_);
    
    if(LoadCurrent < 0.01 && LoadCurrent>-0.01){//
        *force_dir = 0;
        *force_mag = 0;//
    }
    else if (LoadCurrent < 0)
    {   

        *force_dir = 1;
        *force_mag = -((float)LoadCurrent)*LOAD_CURRENT_SCALER;
    }
    else if (LoadCurrent > 0)
    {
        *force_dir = 0;
        *force_mag = ((float)LoadCurrent)*LOAD_CURRENT_SCALER;
    }
    return *force_mag;
}