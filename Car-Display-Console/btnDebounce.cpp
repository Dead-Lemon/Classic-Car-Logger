#include "btnDebounce.h"

Button::Button(uint8_t pin) {
  this-> pin = pin; //this-> differentiates between the pin created in the class and locally, Does this move make it available for the fucntions in the class?
  lastState = LOW;
  init();
}

void Button::init() {
  pinMode(pin, INPUT);
  update();
}


void Button::update() {
  uint8_t newState = digitalRead(pin);

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
