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


struct gyroDataStruct {
  
  float Yaw, Pitch, Roll;
  float AccX,AccY,AccZ;
  float GyroX,GyroY,GyroZ;
  float LinX,LinY,LinZ;
  
} gyroData; //all data that will be logged

struct gyroSendStruct {
  
  float Yaw, Pitch, Roll;
  float AccY, GyroY, LinY;
  
} gyroSend; //all data that will be sent to display

struct gpsDataStruct {
  
    float Long, Lat;
    unsigned long fix_age;
    unsigned long date, time, age;
    float speed, alt, course;
    
} gpsData; //all data that will be logged

struct gpsSendStruct {
  
    float speed, alt, course;  
  
} gpsSend; //all data that will be sent to display


float gpsLong, gpsLat, gpsSpd;
unsigned short gpsSat;
unsigned long gpsAge;
float gpsTime;

void setup() {
  Serial.begin(115200); //used for debug and programming
  Serial1.begin(9200); //interface with GPS module
  Serial2.begin(115200); //send data to display console
  consoleData.begin(Serial2);
  Wire.begin(); //interface with MPU9250
  
  delay(200);
  mpu.setup(0x68);
  delay(200);
  mpu.calibrateAccelGyro();
//  mpu.calibrateMag();
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
   gps.f_get_position(&gpsData.Lat, &gpsData.Long, &gpsData.fix_age);
   gps.get_datetime(&gpsData.date, &gpsData.time, &gpsData.age);
   gpsData.speed = gps.f_speed_kmph();
   gpsData.alt = gps.f_altitude();
   gpsData.course = gps.f_course();
  

}

void serialEvent1() { //hardware serial interupt when data arrives
  gps.encode(Serial1.read()); //update gps on serialEvent
  
}

void consoleUpdate() {

  uint16_t sendSize = 0; //create variable to keep track of number of bytes being sent
  sendSize = consoleData.txObj(gyroSend, sendSize); //pack 1st struct into the buffer
  sendSize = consoleData.txObj(gpsSend, sendSize); //pack 2nd struct into the buffer
  consoleData.sendData(sendSize); //send buffer

}
