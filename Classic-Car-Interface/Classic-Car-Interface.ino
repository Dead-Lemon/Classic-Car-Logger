//classic car interface and logger for racing stats.
//Collected data is stored to SD and live stats sent to display module
//base on STM32F01C8 (blue pull) due to larger rom and multiple hardware serial IO


#include "MPU9250.h" //gyro interface
#include "TinyGPS.h" //gps interface
#include "SerialTransfer.h" //data transfer via serial
#include "STM32TimerInterrupt.h" //hardware timer interupts

#include "dataStruct.h" //imports the data strutures used to logging and display console coms
#include "readSensors.h"
#include "IOmapping.h"

#include "SPI.h"
#include "SdFat.h"
#define SD1_CONFIG SdSpiConfig(SDsel, SHARED_SPI, SD_SCK_MHZ(18), &SPI)
SdFs sd1;
FsFile logFile;
FsFile logDir;
uint32_t logFileNum = 0; //number used to create next log file

//setting up RPM pulse counting
uint32_t tachoCount = 0;
const uint32_t SampleRate = 250; //get latest values from all sensors
const float engineCyclders = 4.0; //set number of cyclynders, 1 tacho pulse = 1 piston firing, 4 pistons = 4 pulse per rev.
bool gpsNewData = false;

//set engine temp sensor pin
const uint16_t engineTempR1 = 4000; //set the resistor value used in the voltage divider circuit
const float engineTempVcc = 3.3; // voltage used in divider circuit.

//set start location, defail is london XD
//the start locaiton is set for recording laps
double startLAT = 51.508131, startLON = -0.128002;
uint16_t lapCounter = 0;
uint32_t lapDebounce = 30000; // 30s before it can check again when lap is found
uint32_t lapDebounceMark = 0; //save current millis to mark delay start point
bool leftStart = false; //check to see car has left start area, to avoid false positive in the pit
float lapPassDist = 50; //50m from start before new lap can trigger
float lapTriggerDist = 5; // trigger within 5 meters

//set up hardware timers for sampling
STM32Timer HWTimer1(TIM1); //enable hardware timer for updating everything

MPU9250 mpu; //gyro module
TinyGPS gps; //gps data parsing
SerialTransfer consoleData; //allow tranfer of data structures over serial
auto fileName = logFileNum + ".csv";


HardwareSerial Serial2(PA3, PA2); //enable serial port 2

void setup() {
  
  Serial.begin(115200); //Start debug interface
  Serial1.begin(9200); //start listening to GPS serial updates
  Serial2.begin(115200); //start display console serial interface
  consoleData.begin(Serial2); //start data exchange with display console
  Wire.begin(); //interface with MPU9250

  sd1.begin(SD1_CONFIG); //start sdcard interfdace

 uint16_t rootFileCount = 0;
 logDir.open("/"); 
 while (logFile.openNext(&logDir, O_RDONLY)) {
    if (!logFile.isHidden()) {
      rootFileCount++;
    }
    logFile.close();
 }
 logFileNum = rootFileCount++;
 fileName = rootFileCount+".csv"; //not sure if using auto is a good idea?
 if (logFile.open(fileName, O_CREAT | O_WRITE | O_APPEND)) { //print csv header
  logFile.print(csvHead);
  logFile.sync();
  logFile.close();
 }
   
  
  delay(200);
  mpu.setup(0x68); //connect to mpu
  delay(200);
  mpu.calibrateAccelGyro(); //calibrate
  mpu.calibrateMag();
  delay(200);
  
  pinMode(tachoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tachoPin), tachoUpdate, FALLING); //enable interupt handling for tacho counting, calls tachoUpdate when falling trigger is detected

  HWTimer1.attachInterruptInterval(SampleRate * 1000, updateAll); //hardware timer to update coms and logs
  
}

void loop() {
// nothing to loop at the moment. may change...
}

void updateAll() {
  
  rpmUpdate();
  sensorUpdate();
  mpuUpdate();
  gpsUpdate();
  consoleUpdate();

}

void rpmUpdate() {

  engineSensor.rpm = ((float)tachoCount/ engineCyclders / (1/(float)SampleRate) * 60.0f); 
  tachoCount = 0;
  
}

void sensorUpdate() {
  engineSensor.engineTemp = readEngineTemp(analogRead(engineTempInput), engineTempR1, engineTempVcc);
  engineSensor.oilPress = readOilPress(analogRead(oilPressInput));
}


void checkLap() { //checks if close to start mark, only resets after 60s and car has left 50m from start.
  gpsData.laptime = millis() - gpsData.laptimeOffset; 
  if ((gpsData.distanceToStart <= lapTriggerDist) and (leftStart) and ((millis() - lapDebounceMark) < lapDebounce)) {
    gpsData.lastLap = gpsData.laptime; //recored previous laptime
    gpsData.laptimeOffset = millis();
    leftStart = false;
    lapDebounceMark = millis();
    gpsData.laps++;   
  } else if ((!leftStart) and (gpsData.distanceToStart > lapPassDist)) {
    leftStart = true;
  }

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
    gpsData.distanceToStart = gps.distance_between(gpsData.Lat, gpsData.Long, startLAT, startLON);
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

void writeLogs() {
 //Time,UTC Time,Lap,Sector,Predicted Lap Time,Predicted vs Best Lap,GPS_Update,GPS_Delay,Latitude,Longitude,Altitude (m),speed (KPH),Heading,Accuracy (m),Accel X,Accel Y,Accel Z,Brake (calculated),Engine Speed (RPM),Throttle Position (%),Brake (%)"};

 if (logFile.open(fileName, O_CREAT | O_WRITE | O_APPEND)) { //print csv header
  logFile.print(millis());  //cpu time
  logFile.print(",");
  logFile.print(gpsData.date); //date
  logFile.print(" ");
  logFile.print(gpsData.time);  //time
  logFile.print(",");
  logFile.print(gpsData.laps);  //lap count
  logFile.print(",");
  logFile.print(gpsData.laptime); //predicted lap time
  logFile.print(",");
  logFile.print(gpsData.laptime - gpsData.lastLap); //prediceted vs best (need to look act saving best time)
  logFile.print(",");
  logFile.print(gpsNewData);  //GPS_Update, show if GPS returned new info, as it only updates 1/sec
  logFile.print(",");
  logFile.print(gpsData.age);  //GPS_Delay
  logFile.print(",");
  logFile.print(gpsData.Lat);  //Latitude
  logFile.print(",");
  logFile.print(gpsData.Long);  //Longitude
  logFile.print(",");
  logFile.print(gpsData.alt);  //Altitude
  logFile.print(",");
  logFile.print(gpsData.speed);  //speed
  logFile.print(",");
  logFile.print(gpsData.course);  //Heading
  logFile.print(",");
  logFile.print(gyroData.AccX);  //Accel X
  logFile.print(",");
  logFile.print(gyroData.AccY);  //Accel Y
  logFile.print(",");
  logFile.print(gyroData.AccZ);  //Accel Z
  logFile.print(",");
  logFile.print(0);  //Brake
  logFile.print(",");
  logFile.print(0);  //throttle
  logFile.print(",");
  logFile.print(gyroData.Roll);  //roll
  logFile.print(",");
  logFile.print(gyroData.Pitch);  //pitch
  logFile.print(",");
  logFile.print(gyroData.Yaw);  //yaw

  logFile.sync();
  logFile.close();
 }
  
}
