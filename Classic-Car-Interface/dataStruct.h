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
    unsigned short satellites;
    float speed, alt, course;
    
} gpsData; //all data that will be logged

struct gpsSendStruct {
  
    float speed, alt, course;
    unsigned short satellites;
  
} gpsSend; //all data that will be sent to display

struct deviceStatus {

  bool ok;
  
} devState;

struct deviceRequest {

  bool start;
  
} devReq;

struct sensorData {

  float batVolt, engineTemp, rpm;
  
} engineSensor;
