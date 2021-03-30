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

struct loggingStruct
{
  uint8_t slaveID;
  uint8_t senseorder;
  bool warnstatus;
  uint8_t sensefault;
  uint8_t senseglyph;
  float sensevals;
  int8_t units;
  const float sensemaxvals;
  const float senseminvals;
  const float sensewarnhivals;
  const float sensewarnlowvals;
  const char sensename[13];
} logger;
