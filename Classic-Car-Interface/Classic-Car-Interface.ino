//classic car interface and logger for racing stats.
//Collected data is stored to SD and live stats sent to display module
//base on STM32F01C8 (blue pull) due to larger rom and multiple hardware serial IO


#include "MPU9250.h" //gyro interface
#include "TinyGPS.h" //gps interface
#include "SerialTransfer.h" //data transfer via serial
#include "STM32TimerInterrupt.h" //hardware timer interupts
#include "dataStruct.h" //imports the data strutures used to logging and display console coms


//setting up RPM pulse counting
uint32_t tachoCount = 0;
uint32_t tachoAVG[4];
const uint32_t tachoSampleRate = 250000; //calculate rpm every 250ms
const uint8_t engineCyclders = 4; //set number of cyclynders, 1 tacho pulse = 1 piston firing, 4 pistons = 4 pulse per rev.
float rpm[4];
float rpmAVG = 0;
const uint8_t rpmSampleSize = 4;
uint8_t rpmPos = 0;
const uint8_t tachoPin = PA1;

//set up hardware timers for sampling
STM32Timer HWTimer1(TIM1); //enable hardware timer for rpm handling
STM32Timer HWTimer2(TIM2); //enable hardware timer for 

MPU9250 mpu; //gyro module
TinyGPS gps; //gps data parsing
SerialTransfer consoleData; //allow tranfer of data structures over serial

HardwareSerial Serial2(PA3, PA2); //enable serial port 2

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
  
  pinMode(tachoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tachoPin), tachoUpdate, FALLING); //enable interupt handling for tacho counting

  HWTimer1.attachInterruptInterval(tachoSampleRate, sampleRPM); //hardware timer to take rpm samples
  HWTimer2.attachInterruptInterval(tachoSampleRate, updateAll); //hardware timer to update coms and logs


}

void loop() {

}

void updateAll() {
  mpuUpdate();
  gpsUpdate();
  rpmUpdate();
  consoleUpdate();

}

void rpmUpdate() {
  float tmp = 0;
  
  for (int i = 0; i<4; i++) {
    tmp += rpm[i];
  }

  engineSensor.rpm = tmp/4.0f;
  
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

        gyroSend.Yaw = mpu.getYaw();
        gyroSend.Pitch = mpu.getPitch();
        gyroSend.Roll = mpu.getRoll();
        gyroSend.AccY = mpu.getAccY(); 
        gyroSend.GyroY = mpu.getGyroY(); 
        gyroSend.LinY = mpu.getLinearAccY(); 

 }
}


void gpsUpdate() {
  
   gps.f_get_position(&gpsData.Lat, &gpsData.Long, &gpsData.fix_age);
   gps.get_datetime(&gpsData.date, &gpsData.time, &gpsData.age);
   gpsData.speed = gps.f_speed_kmph();
   gpsData.alt = gps.f_altitude();
   gpsData.course = gps.f_course();
   gpsData.satellites = gps.satellites();

   gpsSend.speed = gps.f_speed_kmph(); 
   gpsSend.alt = gps.f_altitude();
   gpsSend.course = gps.f_course();
   gpsSend.satellites = gps.satellites();
  

}

void serialEvent1() { //hardware serial interupt when data arrives
  gps.encode(Serial1.read()); //update gps on serialEvent
  
}

void consoleUpdate() {

  uint16_t sendSize = 0; //create variable to keep track of number of bytes being sent
  sendSize = consoleData.txObj(gyroSend, sendSize); //pack 1st struct into the buffer
  sendSize = consoleData.txObj(gpsSend, sendSize); //pack 2nd struct into the buffer
  sendSize = consoleData.txObj(devState, sendSize); 
  sendSize = consoleData.txObj(devReq, sendSize); 
  sendSize = consoleData.txObj(engineSensor, sendSize); 
  consoleData.sendData(sendSize); //send buffer

}

void tachoUpdate() {
  tachoCount++;
}

void sampleRPM() {
 if (rpmPos >= rpmSampleSize) {
  rpmPos = 0;
 }
 
 rpm[rpmPos] = ((float)tachoCount/60.0f); // (tacho count / 4 cylinders / 0.25s) * 60s = rpm

  
}
