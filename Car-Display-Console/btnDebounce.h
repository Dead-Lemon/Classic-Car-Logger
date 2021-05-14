#ifndef BTN_DEBOUNCE_H
#define BTN_DEBOUNCE_H

#include <Arduino.h>

class Button {
  private:
    uint8_t pin;
    bool state, lastState;
    uint32_t prevTime = 0;
    uint32_t debounceDelay = 50; 

  public:
    Button(uint8_t pin);

    void init();
    void update();

    bool getState();
    bool isPressed();
    
};

#endif
