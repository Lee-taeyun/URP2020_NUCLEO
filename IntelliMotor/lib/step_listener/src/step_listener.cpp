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

}
void StepListener::readyToListen(){
    //Please Call once in setup routines

    //Interrrupt Service Routines are registered
    //C++ labmda expression is used -> see here : https://modoocode.com/196

    //Meausre stepSpeed per tickDuration
    speedometer.attach([this](void)->void{
        this->lastStepTicksPerDuration = this->stepTicks;
        this->stepTicks = 0;
    }, tickDuration);

    stepIn->write(0);
    dirIn->write(0);
    ms1In->write(0);
    ms2In->write(0);
    ms3In->write(0);

    this->step->mode(PullDown);
    this->step->rise( [this](void)->void{
        this->stepIn->write(1);
        this->stepTicks++;
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
}

double StepListener::currentSpeedStepsPerSeconds(){
    return this->lastStepTicksPerDuration/tickDuration;
}