float readEngineTemp(uint16_t raw, uint16_t divisor, float Vin) //calculate the tempreture from NTC sensor, like ERR2081
{
  float Vout = 0;  // variable to store the output voltage
  int R2 = 1000;   // variable to store the R2 value
  float Ro = 2012.2; //resistance @25c
  float Rth = 0;   // variable to store the thermistor value

  Vout = (Vin / divisor) * raw;     // Calculates the Voltage on the Input PIN
//  Rth = ((R2 * Vin) / Vout) - R2;   // Calculates the Resistance of the Thermistor
  Rth = (divisor * Vout) / (Vin - Vout);
  float steinhart;                  // This next stage calculates the temp from the resistance
  steinhart = Rth / Ro;         // (R/Ro)  therm @ 25C = 1986
  steinhart = log(steinhart);       // ln(R/Ro)
  steinhart /= 3502;                // 1/B * ln(R/Ro) b coefficient = 3344
  steinhart += 1.0 / (25 + 273.15); // + (1/To) nominal temp is 25c
  steinhart = 1.0 / steinhart;      // Invert
  steinhart -= 273.15;              // convert to C

  return (steinhart);
}

float readOilPress(uint16_t raw)
{
  int rawval;
  long kpaval;
  long oilpress;
  //100 psi Chinese Transducer
  //Sensor slope = MaxV - MinV / MaxP - MinP    ==   3000 - 333 / 6894 - 0   === 0.386 mv per millibar
  //mv per ADC = 3300mv/4095 = 0.80586
  //mb per ADC = 0.80586/0.386   == 2.0877
  //offset = 330/0.80586 = 409 ADC's
  kpaval = ((raw - 409) * 2.0877) / 10;         // convert to kpa
  oilpress = (kpaval * 0.145038); //Convert to psi - sensor is already relative to atmospheric
  return (oilpress);
}

float readBatteryVolt(uint16_t raw)
{
  uint16_t scaled;
  scaled = map(raw,0,1023,0,1860);
  return(float(scaled)/100.0f);
}
