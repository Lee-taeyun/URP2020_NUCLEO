#include "AccelStepper.h"
#include "def_pins.h"


#define SAMPLES 256
//PINMODE IN SETUP
void Get_Linear_Regression(AccelStepper& stepper); //0~400 microstepping, 400~1000 no microstepping
void Get_Linear_Regression2(AccelStepper& stepper);
void Get_Linear_Regression3(AccelStepper& stepper);
double LPF(double data, double alpha, double prev_lpf, int first_time);//If it's first_time LPF called, first_time ==1

//void Set_motor(AccelStepper stepper,double speed, int dir);  //0~400     -> MS1:HIGH, SPEED*2  .. 400~1000  -> MS1:LOW, SPEED
double get_speed();
double calculate_current(int speed); // calcualte the current which is caused by angular speed
int section_sum();
double Read_LPF_current();
