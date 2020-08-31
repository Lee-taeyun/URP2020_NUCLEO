#include "Stall_Load_Detection.h"


#define CURRENT_LEN 500
#define X_SUM 249500 //0~998 SUM OF MULTIPLES OF 2
#define X_SQSUM 166167000

double A, b;
extern Timer t;
AnalogIn current(current_sense);
double Read_LPF_current(){
 
  static int first_ = 1;
  static double prev_lpf = 0;

  if (first_ == 1){
    prev_lpf = LPF(current.read(), 0.99, prev_lpf, first_);
    first_ = 0;
    return prev_lpf;
  }
  else{
    return prev_lpf = LPF(current.read(), 0.99, prev_lpf, first_);
  }
}

double LPF(double data, double alpha, double prev_lpf, int first_time)
{
  if (first_time == 1)
  {
    return data;
  }
  else
  {
    return alpha * prev_lpf + (1 - alpha) * data;
  }
}

int section_sum()
{

  int sum = 0;
  for (int i = 0; i < SAMPLES; i++){
    sum = sum + current.read()*1000;
  }
  printf("%d\n",sum);
}



double get_speed()
{
  static unsigned long last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
  unsigned long temp = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time;

  last_time = temp + last_time;
  return (double)1 / temp * 1000000;
}

double calculate_current(int speed)
{

  return speed * A + b;
}

void Get_Linear_Regression2(AccelStepper& stepper)
{
  double _current[CURRENT_LEN];

  int speed = 0;
  int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();

  while (speed < 1000){
    
    stepper.setSpeed(speed);
    stepper.runSpeed();
    Read_LPF_current();
    if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > 10){
      stepper.runSpeed();
      _current[speed / 2] = Read_LPF_current()*1000;
      speed = speed + 2;
      last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      stepper.runSpeed();
    }
  }
  /*
  double X_ = X_SUM;
  double X_sq =X_SQSUM;
  double Y_ = 0;
  double XY_ = 0;
  for (int i = 0; i < CURRENT_LEN; i++){
    Y_ = Y_ + _current[i];
    XY_ = XY_ + _current[i]*(2*i);
  }
  
  double denum = CURRENT_LEN*X_sq-(X_*X_);
  b = (Y_*X_sq - X_*XY_)/denum;
  A=(500*XY_- X_*Y_)/denum;
*/
/*
  for (int i = 0; i < CURRENT_LEN; i++)
  {
    printf("%d\n",(int)(_current[i]));
  }
  */
  printf("%d\n",(int)(stepper.speed()));
  return;
}

void Get_Linear_Regression3(AccelStepper &stepper)
{
  double X_ = X_SUM;
  double X_sq =X_SQSUM;
  double Y_ = 0;
  double XY_ = 0;

  int speed = 0;
  int last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
  double temp;
  while (speed < 1000){
    
    stepper.setSpeed(speed);
    stepper.runSpeed();
    Read_LPF_current();
    if (std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() - last_time > 10){
      stepper.runSpeed();
      temp = Read_LPF_current();
      Y_ = Y_+temp;
      XY_ = XY_ + temp*speed;
      speed = speed + 2;
      last_time = std::chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      stepper.runSpeed();
    }
  }

  
  double denum = CURRENT_LEN*X_sq-(X_*X_);
  b = (Y_*X_sq - X_*XY_)/denum;
  A=(CURRENT_LEN*XY_- X_*Y_)/denum;


  printf("%d",1000*b);
  printf("%d",1000*A);
}