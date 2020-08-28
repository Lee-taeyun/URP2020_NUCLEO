#include <mbed.h>
#include "step_listener.h"
int main() {
  StepListener driver;
  driver.readyToListen();

  while(1) {
    // put your main code here, to run repeatedly:
  }
}