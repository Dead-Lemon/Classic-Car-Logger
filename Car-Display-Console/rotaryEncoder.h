#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
  private:
    uint8_t clk, dt;
    bool clkState, dtState, clkLastState, dtLastState;
    int8_t direction = 0;
    int32_t counter = 0;

  public:
    Encoder(uint8_t clk, uint8_t dt);
    void init();
    void update();
    void zero();
    int32_t getCount();
    void setCount(int32_t value);
    int8_t getDir();
    
};

#endif
