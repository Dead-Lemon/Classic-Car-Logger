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
const uint32_t SampleRate = 250; //get latest values from all sensors
const float engineCyclders = 4.0; //set number of cyclynders, 1 tacho pulse = 1 piston firing, 4 pistons = 4 pulse per rev.
float rpm;
const uint8_t tachoPin = PA1;

bool gpsNewData = false;

//set up hardware timers for sampling
STM32Timer HWTimer1(TIM1); //enable hardware timer for updating everything

MPU9250 mpu; //gyro module
TinyGPS gps; //gps data parsing
SerialTransfer consoleData; //allow tranfer of data structures over serial

HardwareSerial Serial2(PA3, PA2); //enable serial port 2

void setup() {
  Serial.begin(115200); //used for debug and programming
  Serial1.begin(9200); //interface with GPS module
  Serial2.begin(115200); //send data to display console
  consoleData.begin(Serial2); //start data exchange with display console
  Wire.begin(); //interface with MPU9250
  
  delay(200);
  mpu.setup(0x68); //connect to mpu
  delay(200);
  mpu.calibrateAccelGyro(); //calibrate
  mpu.calibrateMag();
  delay(200);
  
  pinMode(tachoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tachoPin), tachoUpdate, FALLING); //enable interupt handling for tacho counting

  HWTimer1.attachInterruptInterval(SampleRate * 1000, updateAll); //hardware timer to update coms and logs


}

void loop() {
// nothing to loop at the moment. may change...
}

void updateAll() {
  
  rpmUpdate();
  mpuUpdate();
  gpsUpdate();
  consoleUpdate();

}

void rpmUpdate() {

  engineSensor.rpm = ((float)tachoCount/ engineCyclders / (1/(float)SampleRate) * 60.0f); 
  tachoCount = 0;
  
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
   if (gpsNewData) {
    gps.f_get_position(&gpsData.Lat, &gpsData.Long, &gpsData.fix_age);
    gps.get_datetime(&gpsData.date, &gpsData.time, &gpsData.age);
    gpsData.speed = gps.f_speed_kmph();
    gpsData.alt = gps.f_altitude();
    gpsData.course = gps.f_course();
    gpsData.satellites = gps.satellites();

    gpsNewData = false;
   }

}

void serialEvent1() { //hardware serial interupt when data arrives
  gps.encode(Serial1.read()); //update gps on serialEvent
  gpsNewData = true;
  
}

void consoleUpdate() {

  uint16_t sendSize = 0; //create variable to keep track of number of bytes being sent
  sendSize = consoleData.txObj(gyroData, sendSize); //pack 1st struct into the buffer
  sendSize = consoleData.txObj(gpsData, sendSize); //pack 2nd struct into the buffer
  sendSize = consoleData.txObj(devState, sendSize); 
  sendSize = consoleData.txObj(engineSensor, sendSize); 
  consoleData.sendData(sendSize); //send buffer

}

void tachoUpdate() {
  tachoCount++;
}
