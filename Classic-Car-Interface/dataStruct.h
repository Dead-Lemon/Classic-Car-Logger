struct gyroDataStruct {
  
  float Yaw, Pitch, Roll;
  float AccX,AccY,AccZ;
  float GyroX,GyroY,GyroZ;
  float LinX,LinY,LinZ;
  
} gyroData; //all data that will be logged

struct gpsDataStruct {
  
    float Long, Lat;
    uint32_t fix_age;
    uint32_t date, time, age;
    uint8_t satellites;
    float speed, alt, course, distanceToStart;
    uint16_t laps = 0;
    
} gpsData; //all data that will be logged

struct deviceStatus {

  bool ok;
  
} devState;

struct deviceRequest {

  bool start;
  
} devReq;

struct sensorData {

  float batVolt, engineTemp, oilPress, rpm;
  
} engineSensor;
