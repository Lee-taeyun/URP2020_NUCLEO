#include "step_listener.h"

StepListener::StepListener():
            step(external_step), dir(external_dir), ms1(external_ms1), ms2(external_ms2), ms3(external_ms3),
            stepIn(internal_step), dirIn(internal_dir), ms1In(internal_ms1), ms2In(internal_ms2), ms3In(internal_ms3){
    //Initialize member pins
    //Interrupt Service Rountines will be registered in StepListener::readyToListen function
}
StepListener::StepListener(PinName step, PinName dir, PinName ms1, PinName ms2, PinName ms3,
                            PinName stepIn, PinName dirIn, PinName ms1In, PinName ms2In, PinName ms3In):
                            step(step), dir(dir), ms1(ms1), ms2(ms2), ms3(ms3),
                            stepIn(stepIn), dirIn(dirIn), ms1In(ms1In), ms2In(ms2In), ms3In(ms3In) {
    //Initialize member pins
    //Interrupt Service Rountines will be registered in StepListener::readyToListen function

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

    stepIn = 0;
    dirIn = 0;
    ms1In = 0;
    ms2In = 0;
    ms3In = 0;

    this->step.mode(PullDown);
    this->step.rise( [this](void)->void{
        this->stepIn=1;
        this->stepTicks++;
    } );
    this->step.fall( [this](void)->void{this->stepIn = 0;} );


    this->dir.mode(PullDown);
    this->dir.rise( [this](void)->void{this->dirIn = 1;} );
    this->dir.fall( [this](void)->void{this->dirIn = 0;} );

    this->ms1.mode(PullDown);
    this->ms1.rise( [this](void)->void{this->ms1In = 1;} );
    this->ms1.fall( [this](void)->void{this->ms1In = 0;} );

    this->ms2.mode(PullDown);
    this->ms2.rise( [this](void)->void{this->ms2In = 1;} );
    this->ms2.fall( [this](void)->void{this->ms2In = 0;} );

    this->ms3.mode(PullDown);
    this->ms3.rise( [this](void)->void{this->ms3In = 1;} );
    this->ms3.fall( [this](void)->void{this->ms3In = 0;} );
}

double StepListener::currentSpeedStepsPerSeconds(){
    return this->lastStepTicksPerDuration/tickDuration;
}