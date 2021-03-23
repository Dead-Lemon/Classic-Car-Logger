//classic car interface and logger for racing stats.
//Collected data is stored to SD and live stats sent to display module
//base on STM32F01C8 (blue pull) due to larger rom and multiple hardware serial IO


#include "MPU9250.h"
#include <TinyGPS.h>
#include "SerialTransfer.h"


MPU9250 mpu; //gyro module
TinyGPS gps; //gps data parsing
SerialTransfer consoleData; //allow tranfer of data structures over serial

HardwareSerial Serial2(PA3, PA2); //enable serial port 2


struct gyroStruct {
  
  float Yaw, Pitch, Roll;
  float AccX,AccY,AccZ;
  float GyroX,GyroY,GyroZ;
  float LinX,LinY,LinZ;
  
} gyroData;

struct gpsStruct {
  
  float gpsLong, gpsLat, gpsSpd;
  unsigned short gpsSat;
  unsigned long gpsAge;
  float gpsTime;
  
} gpsData;


float gpsLong, gpsLat, gpsSpd;
unsigned short gpsSat;
unsigned long gpsAge;
float gpsTime;

void setup() {
  Serial.begin(115200); //used for debug and programming
  Serial1.begin(9200); //interface with GPS module
  Serial2.begin(115200); //send data to display console
  consoleData.begin(Serial2);
  Wire.begin();
  
  delay(200);
  mpu.setup(0x68);
  delay(200);
  
  mpu.calibrateAccelGyro();
  mpu.calibrateMag();
  delay(200);
  

}

void loop() {

  mpuUpdate();
  

  




}

void mpuUpdate() {
    if (mpu.update()) { //update date info from mpu, if available)
        gyroData.Yaw = mpu.getYaw();
        gyroData.Pitch = mpu.getPitch();
        gyroData.Roll = mpu.getRoll();
        gyroData.AccX = mpu.getAccX();
        gyroData.AccY = mpu.getAccY(); 
        gyroData.AccZ = mpu.getAccZ();
        gyroData.GyroX = mpu.getGyroX();
        gyroData.GyroY = mpu.getGyroY(); 
        gyroData.GyroZ = mpu.getGyroZ();
        gyroData.LinX = mpu.getLinearAccX();
        gyroData.LinY = mpu.getLinearAccY(); 
        gyroData.LinZ = mpu.getLinearAccZ();
        
 }
}

void gpsUpdate() {


}

void serialEvent1() { //hardware serial interupt when data arrives
  gps.encode(Serial1.read()); //update gps on serialEvent
  
}

void consoleUpdate() {

  uint16_t sendSize = 0; //create variable to keep track of number of bytes being sent
  sendSize = consoleData.txObj(gyroData, sendSize); //pack 1st struct into the buffer
  sendSize = consoleData.txObj(gpsData, sendSize); //pack 2nd struct into the buffer
  consoleData.sendData(sendSize); //send buffer


}
