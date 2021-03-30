const uint8_t oilPressInput = A1; //as it says in the name.
const uint8_t tachoPin = PA1; //input from distributor negative. recommded using circuit to clean up this input
const uint8_t engineTempInput = A0; //engine temp sensor input from voltage divider

//Serial 0 rx =?? tx ??
//serial 1 rx =?? tx ??
//Serial 2 rx =PA3 tx=PA2

//MPU i2c SDA = ?? SDC = ??

//SD card SPI CLK = ?? MISO ?? MOSI ??
const uint8_t SDsel = PA7; //spi chip select for sd card
