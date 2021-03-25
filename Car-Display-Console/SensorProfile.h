typedef struct
{
  bool senseactive;
  uint8_t slaveID;
  uint8_t senseorder;
  bool warnstatus;
  uint8_t sensefault;
  unsigned char senseglyphs;
  int sensevals;
  const uint8_t senseunits;
  const int sensemaxvals;
  const int8_t senseminvals;
  int sensewarnhivals;
  int sensewarnlowvals;
  const char sensename[13];
} SingleSensor;


const unsigned char degCBMP[] =
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

const unsigned char degBMP[] =
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

const unsigned char kmhBMP[] = "km/h";

const unsigned char metersBMP[] = "m";

const unsigned char rpmBMP = "RPM";

const unsigned char accBMP = "m/s2"

const uint8_t totalsensors = 9; //this is the actual number of definitions 

SingleSensor Sensors[totalsensors] = {
 //active  slaveID  senseorder  warnstatus  sensefault senseglyphs sensevals  units maxvals minvals warnhivals warnlovals  sensename       
  {true,   1,       1,          false,      0,         degCBMP,    0,          1,    120,     0,      29,        0,        "ENG TEMP"}, 
  {true,   2,       2,          false,      0,         kmhBMP,     0,          0,    900,    -40,     750,       0,        "RPM"}, 
  {true,   3,       3,          false,      0,         rpmBMP,     0,          0,    150,    -40,     140,       0,        "Tbox Temp"}, 
  {true,   4,       4,          false,      0,         degBMP,     0,          1,    72,     0,       65,        40,       "Fuel Pressre"}, 
  {true,   5,       5,          false,      0,         degBMP,     0,          1,    72,     0,       60,        20,       "Gbx Pressure"}, 
  {true,   6,       6,          false,      0,         degBMP,     0,          0,    150,    -40,     100,       0,        "Gbx Temp"}, 
  {true,   7,       7,          false,      0,         degBMP,     0,          1,    72,     0,       60,        20,       "Oil Pressure"}, 
  {true,   8,       8,          false,      0,         degBMP,     0,          0,    150,    -40,     100,       0,        "Oil Temp"}, 
  {true,   9,       9,          false,      0,         degBMP,     0,          2,    1,      0,       999,       1,        "Coolant Lvl"} 
};
