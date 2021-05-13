
//set start location, default is london XD
//the start locaiton is set for recording laps
double startLAT = 51.508131, startLON = -0.128002;
uint16_t lapCounter = 0;
uint32_t lapDebounce = 30000; // 30s before it can check again when lap is found
uint32_t lapDebounceMark = 0; //save current millis to mark delay start point
bool leftStart = false; //check to see car has left start area, to avoid false positive in the pit
float lapPassDist = 50; //50m from start before new lap can trigger
float lapTriggerDist = 5; // trigger within 5 meters
float distanceToStart;
uint16_t laps;
uint32_t  laptime, laptimeOffset, lastLap = 0;

void writeLogs() {
 String csvHead = "Time,UTC Time,Lap,Sector,Predicted Lap Time,Predicted vs Best Lap,GPS_Update,GPS_Delay,Latitude,Longitude,Altitude (m),speed (KPH),Heading,Accuracy (m),Accel X,Accel Y,Accel Z,Brake (calculated),Engine Speed (RPM),Throttle Position (%),Brake (%)";
 Serial1.println("writing logs");
 File logFile = SD.open("datalog.txt", FILE_WRITE);  //print csv header
 if (logFile) {
  logFile.print(millis());  //cpu time
  logFile.print(",");
  logFile.print(gpsData.date); //date
  logFile.print(" ");
  logFile.print(gpsData.time);  //time
  logFile.print(",");
  logFile.print(laps);  //lap count
  logFile.print(",");
  logFile.print(laptime); //predicted lap time
  logFile.print(",");
  logFile.print(laptime - lastLap); //prediceted vs best (need to look act saving best time)
  logFile.print(",");
  logFile.print(gpsData.gpsNewData);  //GPS_Update, show if GPS returned new info, as it only updates 1/sec
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
  logFile.println(gyroData.Yaw);  //yaw
  logFile.close();
 }
}

 void checkSD() {
  if (!SD.begin(chipSelect)) {
    Serial1.println("Insert SD card or logging cannot occur");
    devState.SDcardOk = false;
  } else {
    Serial1.println("SD card detected!");
    devState.SDcardOk = true;
  }
}
