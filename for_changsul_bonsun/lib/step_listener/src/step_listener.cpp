#include "step_listener.h"


StepListener::StepListener(InterruptIn* step, InterruptIn* dir, 
                    InterruptIn* ms1, InterruptIn* ms2, InterruptIn* ms3,
                    DigitalOut* stepIn, DigitalOut* dirIn,
                    DigitalOut* ms1In, DigitalOut* ms2In, DigitalOut* ms3In){
    //Initialize member pins
    //Interrupt Service Rountines will be registered in StepListener::readyToListen function
    this->step = step;
    this->dir = dir;
    this->ms1 = ms1;
    this->ms2 = ms2;
    this->ms3 = ms3;
    this->stepIn = stepIn;
    this->dirIn = dirIn;
    this->ms1In = ms1In;
    this->ms2In = ms2In;
    this->ms3In = ms3In;

    this->isstop = true;

}
bool StepListener::isStop(){
    return this->isstop;
}

void StepListener::stopToListen(){
    this->isstop = true;
    this->step->rise(NULL);
    this->step->fall(NULL);
    this->dir->rise(NULL);
    this->dir->fall(NULL);
    this->ms1->rise(NULL);
    this->ms1->fall(NULL);
    this->ms2->rise(NULL);
    this->ms2->fall(NULL);
    this->ms3->rise(NULL);
    this->ms3->fall(NULL);
}

void StepListener::readyToListen(){
    //Please Call once in setup routines

    //Interrrupt Service Routines are registered
    //C++ labmda expression is used -> see here : https://modoocode.com/196


    /*
    //Meausre stepSpeed per tickDuration
    speedometer.attach([this](void)->void{
        this->lastStepTicksPerDuration = this->stepTicks;
        this->stepTicks = 0;
    }, tickDuration);

    
    */
    this->isstop = false;
    stepIn->write(0);
    dirIn->write(0);
    ms1In->write(0);
    ms2In->write(0);
    ms3In->write(0);

    this->step->mode(PullDown);
    this->step->rise( [this](void)->void{
        calculateSpeed();
        this->stepIn->write(1);
        //added by taeyun
    } );
    this->step->fall( [this](void)->void{this->stepIn->write(0);} );


    this->dir->mode(PullDown);
    this->dir->rise( [this](void)->void{this->dirIn->write(1);} );
    this->dir->fall( [this](void)->void{this->dirIn->write(0);} );

    this->ms1->mode(PullDown);
    this->ms1->rise( [this](void)->void{this->ms1In->write(1);} );
    this->ms1->fall( [this](void)->void{this->ms1In->write(0);} );

    this->ms2->mode(PullDown);
    this->ms2->rise( [this](void)->void{this->ms2In->write(1);} );
    this->ms2->fall( [this](void)->void{this->ms2In->write(0);} );

    this->ms3->mode(PullDown);
    this->ms3->rise( [this](void)->void{this->ms3In->write(1);} );
    this->ms3->fall( [this](void)->void{this->ms3In->write(0);} );
    //Initialize last_time too...
    extern Timer t;
    last_time = std::chrono::duration_cast<chrono::microseconds>(t.elapsed_time()).count();
}
/*
double StepListener::currentSpeedStepsPerSeconds(){
    return this->lastStepTicksPerDuration/tickDuration;
}
*/

void StepListener::calculateSpeed(){
    extern Timer t;
    
    duration = std::chrono::duration_cast<chrono::microseconds>(t.elapsed_time()).count() - last_time;

    last_time = duration + last_time;
    speed =  (double)1 / duration * 1000000;//update speed when pulse rises
}

double StepListener::getCurrentSpeed(){
    extern Timer t;
    unsigned long time_passed = std::chrono::duration_cast<chrono::microseconds>(t.elapsed_time()).count() - last_time;

    if(time_passed > 1000000) //if speed didn't update for 1 seconds, it's speed =0;
        return 0;
    else
        return speed;
}
double StepListener::returnSpeed(){
    return speed;
}