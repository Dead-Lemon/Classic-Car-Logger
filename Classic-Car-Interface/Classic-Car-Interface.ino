//classic car interface and logger for racing stats.
//Collected data is stored to SD and live stats sent to display module
//base on STM32F01C8 (blue pull) due to larger rom and multiple hardware serial IO
//uses board library: https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json

#include "MPU9250.h" //gyro interface
#include "TinyGPS.h" //gps interface
#include "SerialTransfer.h" //data transfer via serial
#include "STM32TimerInterrupt.h" //hardware timer interupts

#include "dataStruct.h" //imports the data strutures used to logging and display console coms
#include "readSensors.h"
#include "IOmapping.h"

#include "SPI.h"


//setting up RPM pulse counting
uint32_t tachoCount = 0;
const uint32_t SampleRate = 250; //get latest values from all sensors
const float engineCyclders = 4.0; //set number of cyclynders, 1 tacho pulse = 1 piston firing, 4 pistons = 4 pulse per rev.

//set engine temp sensor pin
const uint16_t engineTempR1 = 4000; //set the resistor value used in the voltage divider circuit
const float engineTempVcc = 3.3; // voltage used in divider circuit.

bool ledState = false;

//set up hardware timers for sampling
STM32Timer HWTimer1(TIM1); //enable hardware timer for updating everything

MPU9250 mpu; //gyro module
TinyGPS gps; //gps data parsing
SerialTransfer consoleData; //allow tranfer of data structures over serial

//HardwareSerial Serial1(PA10, PA9); //enable serial port 1
HardwareSerial Serial2(PA3, PA2); //enable serial port 2
HardwareSerial Serial3(PB11, PB10); //enable serial port 3

void setup() {

  Serial1.begin(115200); //Start debug interface
  Serial1.println("Starting");
  Serial2.begin(9200); //start listening to GPS serial updates
  Serial3.begin(115200); //start display console serial interface
  consoleData.begin(Serial3); //start data exchange with display console
  Wire.begin(); //interface with MPU9250

  Serial1.println("Initializing MPU");
  delay(200);
  //mpu.setup(0x68); //connect to mpu
  delay(200);
  //mpu.calibrateAccelGyro(); //calibrate Disabled as result is strange output post operation (may be due to other IO not being used on MPU breakout board?), easier to just zero current values
  //mpu.calibrateMag();
  //delay(200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);

  pinMode(tachoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tachoPin), tachoUpdate, FALLING); //enable interupt handling for tacho counting, calls tachoUpdate when falling trigger is detected

  HWTimer1.attachInterruptInterval(SampleRate * 1000, updateAll); //hardware timer to update coms and logs

}

void loop() {

//nothing to do here for now
}


void updateAll() {
  
  Serial1.println("Updating"); 
  rpmUpdate();
  Serial1.println("rpm");
  Serial1.println(engineSensor.rpm);
  sensorUpdate();
  Serial1.println("temp");
  Serial1.println(engineSensor.engineTemp);
  //mpuUpdate();
  gpsUpdate();
  consoleUpdate();
  gpsData.gpsNewData = false;  //set flag to false post update of the console, so the logs can reflect a gps update
  digitalWrite(LED_BUILTIN, ledState); //status to show something is happening, lol
  ledState = !ledState;

}

void rpmUpdate() {

  engineSensor.rpm = ((float)tachoCount/ engineCyclders / (1/(float)SampleRate) * 60.0f);
  tachoCount = 0;

}

void sensorUpdate() {
  engineSensor.engineTemp = readEngineTemp(analogRead(engineTempInput), engineTempR1, engineTempVcc);
  engineSensor.oilPress = readOilPress(analogRead(oilPressInput));
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
//     gyroData.LinX = mpu.getLinearAccX();
//     gyroData.LinY = mpu.getLinearAccY();
//     gyroData.LinZ = mpu.getLinearAccZ(); not logging linear acc.
 }
}


void gpsUpdate() {
   if (gpsData.gpsNewData) {
    gps.f_get_position(&gpsData.Lat, &gpsData.Long, &gpsData.fix_age);
    gps.get_datetime(&gpsData.date, &gpsData.time, &gpsData.age);
    gpsData.speed = gps.f_speed_kmph();
    gpsData.alt = gps.f_altitude();
    gpsData.course = gps.f_course();
    gpsData.satellites = gps.satellites();
   }

}

void serialEvent2() { //hardware serial interupt when data arrives
  gps.encode(Serial2.read()); //update gps on serialEvent
  gpsData.gpsNewData = true;

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
