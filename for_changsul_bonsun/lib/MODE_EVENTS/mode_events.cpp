#include "mode_events.h"

MODES Determine_MODE(){
    if(ms1==0 && ms2 ==0)
        return DEFAULT;
    else if(ms1==0 && ms2 ==1)
        return ALARM;
    else if(ms1==1 && ms2 ==0)
        return STEP_LISTENER;
    else
    {
        return DEFAULT;
    }
    
}

EVENTS Determine_EVENT(MODES MODE){
    if(MODE == DEFAULT){
        if(force_mag_ > 0 && force_dir_ ==0)
            return OP_DIR;
        else if(force_mag_ >0 && force_dir_==1)
            return SAME_DIR;
    }

    return NOTHING;
}


void SAME_DIR_EVENT(AccelStepper* stepper){
    int speed = stepper->speed();
    if( speed >0){
        stepper->setSpeed(speed+100);
    }
    else{
        stepper->setSpeed((speed-100));
    }
}
void OP_DIR_EVENT(AccelStepper* stepper){
    int speed = stepper->speed();
    if( speed > 0)
        stepper->setSpeed(-(speed-10));
    if( speed < 0)
        stepper->setSpeed(-(speed+10));
}