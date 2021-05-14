#include "rotaryEncoder.h"
#include "btnDebouce.h"

Encoder::Encoder(uint8_t clk, dt, btn) {
  this-> clk = clk; //this-> differentiates between the pin created in the class and locally, I assume this is a lazy/easy way of not creating a new var with a different name? 
  this-> dt = dt;
  this-> btn = btn;
  lastState = LOW;
  init();
}

void Encoder::init() {
  pinMode(clk, INPUT);
  attachInterrupt(digitalPinToInterrupt(clk), update, CHANGE);
  
  pinMode(dt, INPUT);
  pinMode(btn, INPUT_PULLUP);
  update();
}


void Encoder::update() {
  clkState = digitalRead(clk);

  if (newState != lastState) {
    prevTime = millis();
  }

  if (millis() - prevTime > debounceDelay) {
    state = newState;
  }

  lastState = newState;
}

bool Button::getState() {
  update();
  return state;
}

bool Button::isPressed() {
  return (getState() == HIGH); //can be used to invert button state by setting to LOW
}
