#include "Stall_Load_Detection.h"

#define CURRENT_LEN 500
#define X_SUM 249500 //0~998 SUM OF MULTIPLES OF 2
#define X_SQSUM 166167000

double A, b, A_m, b_m;

double Read_LPF_current(){
  static int first_ = 1;
  static double prev_lpf = 0;
  int data = analogRead(READ_CURRENT);
  if (first_ == 1){
    prev_lpf = LPF(data, 0.99, prev_lpf, first_);
    first_ = 0;
    return prev_lpf;
  }
  else{
    return prev_lpf = LPF(data, 0.99, prev_lpf, first_);
  }
}

double LPF(int data, double alpha, double prev_lpf, int first_time)
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
    sum = sum + analogRead(READ_CURRENT);
  }
  Serial.println(sum);
}

void Set_motor(AccelStepper stepper, double speed, int dir){
  if (speed < 400){
    digitalWrite(SET_MS1, HIGH);
    stepper.setSpeed(speed * 2 * dir);
  }
  else
  {
    digitalWrite(SET_MS1, LOW);
    stepper.setSpeed(speed * dir);
  }
}

double get_speed()
{
  static unsigned long last_time = micros();
  unsigned long temp = micros() - last_time;

  last_time = temp + last_time;
  return (double)1 / temp * 1000000;
}

double calculate_current(int speed)
{
  if (speed < 400)
  {
    return speed * A_m + b_m;
  }
  return speed * A + b;
}

/*
void moving_sum(int group){
  //check if the array is initially full
  static int first=1; // check if it's the first time
  static int mov_sum=0;
  static int replace_index=0;

  if( first ==1 ){// if it's first time
    for( int i=0; i<SAMPLES; i++){
      vReal[i] = analogRead(A0);
      mov_sum = mov_sum+vReal[i];
    }
    Serial.println((float)mov_sum/SAMPLES);
    first = 0;
    replace_index=0;
  }
  else{
    for(int i=0; i<group; i++){
      mov_sum = mov_sum - vReal[replace_index];
      vReal[replace_index] = analogRead(A0);
      mov_sum = mov_sum + vReal[replace_index];
      replace_index =  (replace_index+1) % SAMPLES;
    }
    
    Serial.println((float)mov_sum/SAMPLES);
    
  }
}
*/

void Get_Linear_Regression2(AccelStepper &stepper)
{
  double _current[CURRENT_LEN];

  int speed = 0;
  int last_time = millis();

  while (speed < 1000){
    digitalWrite(SET_MS1, LOW);
    stepper.setSpeed(speed);
    stepper.runSpeed();
    Read_LPF_current();
    if (millis() - last_time > 10){
      stepper.runSpeed();
      _current[speed / 2] = Read_LPF_current();
      speed = speed + 2;
      last_time = millis();
      stepper.runSpeed();
    }
  }
  
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


  for (int i = 0; i < CURRENT_LEN; i++)
  {
    Serial.println(_current[i]);
  }
  Serial.println(b,10);//second parameter is precision
  Serial.println(A,10);
}

void Get_Linear_Regression3(AccelStepper &stepper)
{
  double X_ = X_SUM;
  double X_sq =X_SQSUM;
  double Y_ = 0;
  double XY_ = 0;

  int speed = 0;
  int last_time = millis();
  double temp;
  while (speed < 1000){
    digitalWrite(SET_MS1, LOW);
    stepper.setSpeed(speed);
    stepper.runSpeed();
    Read_LPF_current();
    if (millis() - last_time > 10){
      stepper.runSpeed();
      temp = Read_LPF_current();
      Y_ = Y_+temp;
      XY_ = XY_ + temp*speed;
      speed = speed + 2;
      last_time = millis();
      stepper.runSpeed();
    }
  }

  
  double denum = CURRENT_LEN*X_sq-(X_*X_);
  b = (Y_*X_sq - X_*XY_)/denum;
  A=(CURRENT_LEN*XY_- X_*Y_)/denum;


  Serial.println(b,10);//second parameter is precision
  Serial.println(A,10);
}