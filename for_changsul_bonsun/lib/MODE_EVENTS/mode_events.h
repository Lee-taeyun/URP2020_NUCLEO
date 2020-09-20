
#ifndef MODE_EVENTS
#define MODE_EVENTS

#include <mbed.h>
#include "def_pins.h"
#include "AccelStepper.h"

extern InterruptIn ms1;
extern InterruptIn ms2;

extern DigitalOut stall_;
extern DigitalOut force_dir_;
extern AnalogOut force_mag_;

enum EVENTS{SAME_DIR, OP_DIR, SHORT_STROKE,NOTHING};
enum MODES{DEFAULT,ALARM,STEP_LISTENER };
MODES Determine_MODE();
EVENTS Determine_EVENT(MODES MODE);



void SAME_DIR_EVENT(AccelStepper* stepper);
void OP_DIR_EVENT(AccelStepper* stepper);

#endif