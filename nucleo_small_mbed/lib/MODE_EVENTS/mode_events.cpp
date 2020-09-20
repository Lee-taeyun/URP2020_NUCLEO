#include "mode_events.h"

MODES Determine_MODE(){
    if(ms1==0 && ms2 ==0)
        return DEFAULT;
    if(ms1==0 && ms2 ==1)
        return ALARM;
    if(ms1==1 && ms2 ==0)
        return STEP_LISTENER;
}

EVENTS Determine_EVENT(MODES MODE){
    if(MODE != DEFAULT){
        if(force_mag_ > 0 && force_dir_ ==0)
            return SHORT_STROKE;
        if(force_mag_ >0 && force_dir_==1)
            return SAME_DIR;
    }

    return NOTHING;
}