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
    Encoder(uint8_t clk, dt, btn)
    void init();
    void update();
    uint8_t direction();
    bool getState();
    bool isPressed();
    
};

#endif
