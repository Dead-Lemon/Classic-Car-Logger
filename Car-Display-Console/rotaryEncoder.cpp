#include "rotaryEncoder.h"

Encoder::Encoder(uint8_t clk,uint8_t  dt) {
  this-> clk = clk; //this-> differentiates between the pin created in the class and locally, I assume this is a lazy/easy way of not creating a new var with a different name? 
  this-> dt = dt;
  clkLastState = false;
  dtLastState = false;
  init();
}

void Encoder::init() {
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
}

void Encoder::update() {
  
  clkState = digitalRead(clk);
  dtState = digitalRead(dt);

  if (clkState != clkLastState) {
    if ((clkState and dtState) or (!clkState and !dtState)) {
      direction = -1;
    } else {
      direction = 1;
    }
  
    counter += direction; 
    clkLastState = clkState;
  }
}

void Encoder::zero() {//zero the counter
  counter = 0;
}

void Encoder::setCount(int32_t value) {
  counter = value;
}

int8_t Encoder::getDir() {
  int8_t dir = direction;
  direction = 0;
  return dir;
}

int32_t Encoder::getCount(){
  return counter;
}
