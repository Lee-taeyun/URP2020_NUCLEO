#include "step_listener.h"

void StepListener::_stepUp(){this->stepIn=1;};
void StepListener::_stepDown(){this->stepIn=0;};
void StepListener::_dirUp(){this->dirIn=1;};
void StepListener::_dirDown(){this->dirIn=0;};
void StepListener::_ms1Up(){this->ms1In=1;};
void StepListener::_ms1Down(){this->ms1In=0;};
void StepListener::_ms2Up(){this->ms2In=1;};
void StepListener::_ms2Down(){this->ms2In=0;};
void StepListener::_ms3Up(){this->ms3In=1;};
void StepListener::_ms3Down(){this->ms3In=0;};

StepListener::StepListener(PinName step, PinName dir, PinName ms1, PinName ms2, PinName ms3,
                            PinName stepIn, PinName dirIn, PinName ms1In, PinName ms2In, PinName ms3In):
                            step(step), dir(dir), ms1(ms1), ms2(ms2), ms3(ms3),
                            stepIn(stepIn), dirIn(dirIn), ms1In(ms1In), ms2In(ms2In){

    //In python, there is a lambda function exrpression.... 
    //In C++, I dont know the equivalnet expression... please update
    
    this->step.mode(PullDown);
    this->step.rise(&this->_stepUp);
    this->step.fall(&(this->_stepDown));

    this->dir.mode(PullDown);
    this->dir.rise(&(this->_dirUp));
    this->dir.fall(&(this->_dirDown));

    this->ms1.mode(PullDown);
    this->ms1.rise(&(this->_ms1Up));
    this->ms1.fall(&(this->_ms1Down));

    ms2.mode(PullDown);
    ms2.rise(&(this->_ms2Up));
    ms2.fall(&(this->_ms2Down));

    ms3.mode(PullDown);
    ms3.rise(&(this->_ms3Up));
    ms3.fall(&(this->_ms3Down));    
}