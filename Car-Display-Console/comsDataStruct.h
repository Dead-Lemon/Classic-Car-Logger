//expected data structures from car interface module
struct gyroSendStruct {
  
  float Yaw, Pitch, Roll;
  float AccY, GyroY, LinY;
  
} gyroRecv; //all data that will be sent to display

struct gpsSendStruct {
  
    float speed, alt, course;
    unsigned short satellites;
  
} gpsRecv; //all data that will be sent to display

struct deviceStatus {

  bool ok;
  
} devState;

struct deviceRequest {

  bool start;
  
} devReq;

struct sensorData {

  float batVolt, engineTemp, rpm;
  
} sensorRecv;
