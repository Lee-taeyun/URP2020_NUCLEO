
#ifndef MODE_EVENTS_H
#define MODE_EVENTS_H

#include <mbed.h>
#include "def_pins.h"
#include "AccelStepper.h"
#include "stall_load_detector.h"

extern InterruptIn ms1;
extern InterruptIn ms2;

extern DigitalOut stall_;
extern DigitalOut force_dir_;
extern AnalogOut force_mag_;

enum EVENTS{SAME_DIR, OP_DIR, SHORT_STROKE,NOTHING};
enum MODES{DEFAULT, STEP_LISTENER, MOTION_1, CALIBRATION };
MODES determine_mode(InterruptIn* pin1, InterruptIn* pin2);
EVENTS determine_event(StallLoadDetector* detector, double speed);

MODES Determine_MODE();
EVENTS Determine_EVENT(MODES MODE);



void SAME_DIR_EVENT(AccelStepper* stepper);
void OP_DIR_EVENT(AccelStepper* stepper);

#endif