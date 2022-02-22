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
    float speed, alt, course;
    bool gpsNewData = false;
    
} gpsData; //all data that will be logged

struct sensorData {

  float batVolt, engineTemp, oilPress, brake, throttle;
  uint32_t rpm;
  
} engineSensor;
