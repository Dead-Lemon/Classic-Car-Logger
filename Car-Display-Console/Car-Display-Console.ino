#include <ShiftedLCD.h>
#include <SPI.h>
#include <BigNumbers.h>
#include "SerialTransfer.h"

SerialTransfer consoleData;

//expected data structures from car interface module
struct gyroSendStruct {
  
  float Yaw, Pitch, Roll;
  float AccY, GyroY, LinY;
  
} gyroSend; //all data that will be sent to display

struct gpsSendStruct {
  
    float speed, alt, course; 
  
} gpsSend; //all data that will be sent to display

//create lcd instances
LiquidCrystal lcd1(9);
LiquidCrystal lcd2(10);
LiquidCrystal lcd3(11);
LiquidCrystal lcd4(12);

BigNumbers bigNum1(&lcd1);
BigNumbers bigNum2(&lcd2);
BigNumbers bigNum3(&lcd3);
BigNumbers bigNum4(&lcd4);


void setup() {

  Serial.begin(115200);
  consoleData.begin(Serial);
 
  lcd1.begin(16,2); // setup LCD rows and columns
  lcd2.begin(16,2); 
  lcd3.begin(16,2); 
  lcd4.begin(16,2); 
  
  bigNum1.begin(); // set up BigNumbers
  bigNum2.begin();
  bigNum3.begin();
  bigNum4.begin();

  lcd1.clear(); // clear display
  lcd2.clear();
  lcd3.clear();
  lcd4.clear();

  
}

void loop() {

  if(consoleData.available()) {
    uint16_t recSize=0; //create var to track incoming bytes
    recSize = consoleData.rxObj(gyroSend, recSize); //pack 1st struct into the buffer
    recSize = consoleData.rxObj(gpsSend, recSize); //pack 2nd struct into the buffer
  }

  int currentTime = millis() / 100; // assigns the current time since boot in tenths of a second to currentTime
  byte lastDigit = currentTime % 10;
  currentTime = currentTime /= 10;
  bigNum1.displayLargeInt(currentTime, 0, 4, false);
  bigNum2.displayLargeInt(currentTime, 0, 4, false);
  bigNum3.displayLargeInt(currentTime, 0, 4, false);
  bigNum4.displayLargeInt(currentTime, 0, 4, false);
  lcd1.setCursor(12, 1);
  lcd1.print(".");
  lcd1.print(lastDigit);

}
