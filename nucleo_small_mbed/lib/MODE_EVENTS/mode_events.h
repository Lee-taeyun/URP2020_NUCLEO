#include <mbed.h>
#include "def_pins.h"

extern InterruptIn ms1;
extern InterruptIn ms2;

extern DigitalOut stall_;
extern DigitalOut force_dir_;
extern AnalogOut force_mag_;

enum EVENTS{SAME_DIR, OP_DIR, SHORT_STROKE,NOTHING};
enum MODES{DEFAULT,ALARM,STEP_LISTENER };
MODES Determine_MODE();
EVENTS Determine_EVENT(MODES MODE);

stepper1.setSpeed(-speed);
    speed = -speed;

void SAME_DIR_EVENT(AccelStepper* stepper,int* speed){
    stepper->speed = *speed+100;
    *speed = *speed+100;; 
}
void SAME_DIR_EVENT(AccelStepper* stepper,int* speed){
    stepper->speed = -(*speed);
    *speed = -speed; 
}