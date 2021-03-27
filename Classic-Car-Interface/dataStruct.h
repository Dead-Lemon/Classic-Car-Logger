struct gyroDataStruct {
  
  float Yaw, Pitch, Roll;
  float AccX,AccY,AccZ;
  float GyroX,GyroY,GyroZ;
  float LinX,LinY,LinZ;
  
} gyroData; //all data that will be logged

struct gpsDataStruct {
  
    float Long, Lat;
    unsigned long fix_age;
    unsigned long date, time, age;
    unsigned short satellites;
    float speed, alt, course;
    
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
