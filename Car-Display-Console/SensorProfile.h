typedef struct
{
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
} SingleSensor;


const uint8_t totalsensors = 9; //this is the actual number of definitions 

SingleSensor Sensors[totalsensors] = {
 //senseorder  warnstatus  sensefault senseglyphs sensevals  units maxvals minvals warnhivals warnlovals  sensename       
  {1,          false,      0,         1,           0,          1,    120,     0,      29,        0,        "ENG TEMP"}, 
  {2,          false,      0,         2,           0,          0,    900,    -40,     750,       0,        "SPEED"}, 
  {3,          false,      0,         3,           0,          0,    150,    -40,     140,       0,        "RPM"}, 
  {4,          false,      0,         4,           0,          1,    72,     0,       65,        40,       "Acceleration"}, 
  {5,          false,      0,         5,           0,          1,    72,     0,       60,        20,       "Altitude"}, 
  {6,          false,      0,         6,           0,          0,    150,    -40,     100,       0,        "Course"}, 
  {7,          false,      0,         7,           0,          1,    72,     0,       60,        20,       "other"}, 
  {8,          false,      0,         7,           0,          0,    150,    -40,     100,       0,        "Oil Temp"}, 
  {9,          false,      0,         7,           0,          2,    1,      0,       999,       1,        "Coolant Lvl"} 
};

byte degCBMP[] =
{
  B11100,
  B10100,
  B11100,
  B00011,
  B00100,
  B01000,
  B00100,
  B00011
};

char degBMP[8] =
{
  B11100,
  B11100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
