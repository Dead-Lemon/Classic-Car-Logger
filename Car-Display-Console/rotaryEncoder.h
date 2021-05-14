#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
  private:
    uint8_t clk, dt, btn;
    bool clkState, clklastState;
    uint32_t prevTime = 0;
    uint32_t debounceDelay = 50; 

  public:
    Button(uint8_t setPin);

    void init();
    void update();

    bool getState();
    bool isPressed();
    
};

#endif
