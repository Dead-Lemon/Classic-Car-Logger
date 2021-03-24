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
    unsigned short satellites;
  
} gpsSend; //all data that will be sent to display

struct deviceStatus {

  bool ok;
  
} devState;

struct deviceRequest {

  bool start;
  
} devReq;

struct sensorData {

  float batVolt, engineTemp, rpm;
  
} sensor;

struct lcdSettings {

  int brightness;
  bool backlight;
  
} lcdSet; //univeral lcd settings

struct numDisplay {
 
  int big, little = 0;
  int size = 4;
  
} numDisp1, numDisp2, numDisp3, numDisp4; //store converted float for big number display

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
    recSize = consoleData.rxObj(devState, recSize); 
    recSize = consoleData.rxObj(devReq, recSize); 
    recSize = consoleData.rxObj(sensor, recSize); 
  }


  numDisp1.big, numDisp1.little = bigInt(gyroSend.Yaw); //convert float to int and split the real and decimal values
  bigNum1.displayLargeInt(numDisp1.big, 0, 4, false); //draw big number
  printLittle(numDisp1.little, &lcd1); //draw decimal value in small text
  
  numDisp2.big, numDisp2.little = bigInt(gyroSend.Roll);  
  bigNum2.displayLargeInt(numDisp2.big, 0, 4, false);
  printLittle(numDisp2.little, &lcd2);
  
  numDisp3.big, numDisp3.little = bigInt(gpsSend.speed);  
  bigNum3.displayLargeInt(numDisp3.big, 0, 4, false);
  printLittle(numDisp3.little, &lcd3);
  
  numDisp4.big, numDisp4.little = bigInt(gpsSend.satellites);  
  bigNum4.displayLargeInt(numDisp4.big, 0, 4, false);
  printLittle(numDisp4.little, &lcd4);


}

void printLittle(int little, LiquidCrystal *lcd) {
  
  lcd->setCursor(12, 1);
  lcd->print(".");
  lcd->print(little);
} //print the decimal value in small text

int bigInt(float raw) {
  int big, little;
  int tmp;
  tmp = raw * 10; //move the decimal point up 1
  little = (int)tmp % 10; //save that decimal point as the int little
  big = (int)raw/10; //remove the decimal point and save as the int big
  return(big, little);
}
