#include <ShiftedLCD.h>
#include <SPI.h>
#include <BigNumbers.h>
#include "SerialTransfer.h"
#include "SensorProfile.h"
#include "comsDataStruct.h"
#include "IOmapping.h"
#include "menu.h"
#include "SD.h"
#include "logging.h"

uint32_t logFileNum = 0; //number used to create next log file
auto fileName = logFileNum + ".csv";

SerialTransfer consoleData;

struct lcdSettings {

  int brightness;
  bool backlight;
  
} lcdSet; //univeral lcd settings

struct numDisplay {
 
  int32_t big, little = 0;
  int8_t size = 3;
  
} numDisp1, numDisp2, numDisp3, numDisp4; //store converted float for big number display

//create lcd instances
LiquidCrystal lcd1(lcd1_sel);
LiquidCrystal lcd2(lcd2_sel);
LiquidCrystal lcd3(lcd3_sel);
LiquidCrystal lcd4(lcd4_sel);

BigNumbers bigNum1(&lcd1);
BigNumbers bigNum2(&lcd2);
BigNumbers bigNum3(&lcd3);
BigNumbers bigNum4(&lcd4);

uint32_t currentMillis, prevMillis = 0;
uint32_t lcdUpdateRate = 100;

void setup() {

  Serial.begin(115200);
  consoleData.begin(Serial);

  //Serial1.println("Checking For SD Card SD");
  // see if the card is present and can be initialized:
  checkSD();
 
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

  lcd1.print("  SPEEDOPHILE  ");
  lcd2.print("    TOUCHING   ");
  lcd3.print("      YOUR     ");
  lcd4.print(" ...  CAR   ...");

  delay(2000);  

  lcd1.clear(); // clear display
  lcd2.clear();
  lcd3.clear();
  lcd4.clear();
  
}

void loop() {

  currentMillis = millis();

  if(consoleData.available()) {
    uint16_t recSize=0; //create var to track incoming bytes
    recSize = consoleData.rxObj(gyroData, recSize); //pack 1st struct into the buffer
    recSize = consoleData.rxObj(gpsData, recSize); //pack 2nd struct into the buffer
    recSize = consoleData.rxObj(devState, recSize); 
    recSize = consoleData.rxObj(engineSensor, recSize);
    //when new data arrives, write to the SD card
    if (devState.SDcardOk) {
      writeLogs();
    } else {
      checkSD(); 
    }     
  }

  if ((currentMillis - prevMillis) > lcdUpdateRate) {
    numDisp1.big, numDisp1.little = bigInt(gyroData.Yaw); //convert float to int and split the real and decimal values
    bigNum1.displayLargeInt(numDisp1.big, 0, 3, false); //draw big number
    printLittle(numDisp1.little, &lcd1); //draw decimal value in small text
  
    numDisp2.big, numDisp2.little = bigInt(gyroData.Roll);  
    bigNum2.displayLargeInt(numDisp2.big, 0, 3, false);
    printLittle(numDisp2.little, &lcd2);
  
    numDisp3.big, numDisp3.little = bigInt(gpsData.speed);  
    bigNum3.displayLargeInt(numDisp3.big, 0, 3, false);
    printLittle(numDisp3.little, &lcd3);
  
    numDisp4.big, numDisp4.little = bigInt(gpsData.satellites);  
    bigNum4.displayLargeInt(numDisp4.big, 0, 3, false);
    printLittle(numDisp4.little, &lcd4);
    prevMillis = currentMillis;
  }
  


}

void printLittle(int little, LiquidCrystal *lcd) {
  
  lcd->setCursor(9, 1);
  lcd->print(".");
  lcd->print(little);
  
} //print the decimal value in small text

int bigInt(float raw) {
  int big, little;
  float tmp;
  tmp = raw * 10; //move the decimal point up 1
  little = (int)tmp % 10; //save that decimal point as the int little
  big = (int)tmp/10; //remove the decimal point and save as the int big
  return(big, little);
}

void sendData() {

  uint16_t sendSize = 0; //create variable to keep track of number of bytes being sent
  sendSize = consoleData.txObj(devReq, sendSize); //pack 1st struct into the buffer 
  consoleData.sendData(sendSize); //send buffer
}
