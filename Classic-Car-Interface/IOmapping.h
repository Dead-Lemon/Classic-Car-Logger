const uint8_t oilPressInput = PA1; //as it says in the name.
const uint8_t tachoPin = PB12; //input from distributor negative. recommded using circuit to clean up this input
const uint8_t engineTempInput = PA0; //engine temp sensor input from voltage divider
const uint8_t batteryVoltageInput = PB0; //battery voltage input from voltage divider

//Serial 1 rx =PA10 tx PA9
//serial 2 rx =PB11 tx PB10
//Serial 3 rx =PA3 tx=PA2

//MPU i2c SDA = PB7 SCL = PB6
