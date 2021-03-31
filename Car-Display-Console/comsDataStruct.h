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
    uint16_t laps;
    uint32_t  laptime, laptimeOffset, lastLap = 0;
    
} gpsData; //all data that will be logged

struct deviceStatus {

  bool ok, started, marked;
  
} devState;

struct deviceRequest {

  bool start, markHome;
  
} devReq;

struct sensorData {

  float batVolt, engineTemp, oilPress, rpm, brake, throttle;
  
} engineSensor;
