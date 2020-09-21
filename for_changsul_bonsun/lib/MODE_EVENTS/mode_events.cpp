#include "mode_events.h"
MODES determine_mode(InterruptIn* pin1, InterruptIn *pin2){
    int p1 = *pin1;
    int p2 = *pin2;
    if(p1 == 0 && p2 == 0){
        return DEFAULT;
    }else if(p1 == 0 && p2 == 1){
        return STEP_LISTENER;
    }else if(p1 == 1 && p2 == 0){
        return MOTION_1;
    }else if(p1 == 1 && p2 == 0){
        return CALIBRATION;
    }else{
        return DEFAULT;
    }
}
EVENTS determine_event(StallLoadDetector* detector, double speed){
    double loadCurrent = detector->getLoadCurrent(speed);
    if(loadCurrent < 0.01 && loadCurrent > -0.01){
        return NOTHING;
    }else if( loadCurrent*100 < 0){
        return SAME_DIR;
    }else if( loadCurrent*100 > 0){
        return OP_DIR;
    }
    return NOTHING;
}

MODES Determine_MODE(){
    if(ms3==0 && ms2 ==0)
        return DEFAULT;
    else if(ms3==0 && ms2 ==1)
        return STEP_LISTENER;
    else if(ms3==1 && ms2 ==0)
        return MOTION_1;
    else if(ms3==1 && ms2 == 1)
    {
        return CALIBRATION;
    }else{
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