struct gyroDataStruct {
  
  float Yaw, Pitch, Roll;
  float AccX,AccY,AccZ;
  float GyroX,GyroY,GyroZ;
//  float LinX,LinY,LinZ; does linear accelleration even make sense in this project scope? removing for sake of rom space & transfer speeds
  
} gyroData; //all data that will be logged


struct gpsDataStruct {
  
    float Long, Lat;
    uint32_t fix_age;
    uint32_t date, time, age;
    uint8_t satellites;
    float speed, alt, course, distanceToStart;
    uint16_t laps;
    uint32_t  laptime, laptimeOffset, lastLap = 0;
    bool gpsNewData = false;
    
} gpsData; //all data that will be logged

struct deviceStatus {

  bool SDcardOk, ok, started, marked;
  
} devState;

struct deviceRequest {

  bool start, markHome;
  
} devReq;

struct sensorData {

  float batVolt, engineTemp, oilPress, rpm, brake, throttle;
  
} engineSensor;


//csv header
char csvHead[]={"Time,UTC Time,Lap,Sector,Predicted Lap Time,Predicted vs Best Lap,GPS_Update,GPS_Delay,Latitude,Longitude,Altitude (m),speed (KPH),Heading,Accuracy (m),Accel X,Accel Y,Accel Z,Brake (calculated),Engine Speed (RPM),Throttle Position (%),Brake (%)"};
