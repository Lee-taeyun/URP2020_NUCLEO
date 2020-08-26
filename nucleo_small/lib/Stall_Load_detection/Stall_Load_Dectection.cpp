#include "Stall_Load_Detection.h"



double A,b, A_m,b_m;
double Read_LPF_current(){
  static int first_=1;
  static int prev_lpf=0;
  int data = analogRead(READ_CURRENT);
  if(first_ ==1){
    first_ =0;
    return prev_lpf = LPF(data,0.99,prev_lpf,first_); 
  }
  else{
    return prev_lpf = LPF(data,0.99,prev_lpf,first_);
  }
}

void Get_Linear_Regression(AccelStepper& stepper){
    double mic_current[200];
    double _current[300];

    int speed = 0;
    int last_time = millis();
    
    while(speed <400){
        digitalWrite(SET_MS1,HIGH);
        stepper.setSpeed(speed*2);
        stepper.runSpeed();
        Read_LPF_current();
        if(millis()-last_time >10){

            mic_current[speed/2] = Read_LPF_current();
            speed = speed+2;
            last_time = millis();
        }
    }
    while(speed<1000){
        digitalWrite(SET_MS1,LOW);
        stepper.setSpeed(speed);
        stepper.runSpeed();
        Read_LPF_current();
        if(millis()-last_time >10){
            _current[speed/2-200] = Read_LPF_current();
            speed = speed+2;
            last_time = millis();
        }
    }
    
    
    /*    calculate A_m,b_m      */
    double X_m = 195;
    double Y_m =0;
    for(int i=0; i<200 ; i++){
      Y_m = Y_m + mic_current[i]/40.0;
    }
    double numerator=0;
    for(int i=0; i<200 ; i++){
      numerator = numerator + (10*i-X_m)*(mic_current[i]-Y_m);
    }
    double denumerator=0;
    for(int i=0; i<200 ; i++){
      denumerator = denumerator + (10*i-X_m)*(10*i-X_m);
    }
    A_m = numerator/denumerator;
    b_m = Y_m - A_m*X_m;
    //Let's calculate A,b

    double X_ = 695;
    double Y_ =0;
    for(int i=0; i<300 ; i++){
      Y_ = Y_ + _current[i]/60.0;
    }
    numerator=0;
    for(int i=0; i<300 ; i++){
      numerator = numerator + (10*(i+40)-X_)*(_current[i]-Y_);
    }
    denumerator=0;
    for(int i=0; i<300 ; i++){
      denumerator = denumerator + (10*(i+40)-X_)*(10*(i+40)-X_);
    }
    A = numerator/denumerator;
    b = Y_ - A*X_;

    for(int i=0; i<200 ; i++){
      //Serial.println(mic_current[i]);
    }
      Serial.println();
    for(int i=0; i<300 ; i++){
      Serial.println(_current[i]);
    }
   

}

double LPF(int data,double alpha,double prev_lpf,int first_time){
  if(first_time ==1){
    return data;
  }
  else{
    return alpha*prev_lpf + (1-alpha)*data;
  }
}

int section_sum(){
  
  int sum=0;
  for( int i=0; i<SAMPLES; i++){
    ;
    sum = sum+analogRead(READ_CURRENT);
  }
    Serial.println(sum);
}


void Set_motor(AccelStepper stepper,double speed, int dir){
    if(speed <400 ){
        digitalWrite(SET_MS1,HIGH);
        stepper.setSpeed(speed*2*dir);
    }
    else{
        digitalWrite(SET_MS1,LOW);
        stepper.setSpeed(speed*dir);
    }
}


double get_speed(){
    static unsigned long last_time = micros();
    unsigned long  temp = micros()-last_time;

    last_time =  temp+ last_time;
    return (double)1/temp*1000000;
}

double calculate_current(int speed){
    if(speed<400){
        return speed*A_m +b_m;
    }
  return speed*A+b;
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