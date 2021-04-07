const uint8_t oilPressInput = PA1; //as it says in the name.
const uint8_t tachoPin = PA1; //input from distributor negative. recommded using circuit to clean up this input
const uint8_t engineTempInput = PA0; //engine temp sensor input from voltage divider

//Serial 1 rx =PA10 tx PA9
//serial 2 rx =PB11 tx PB10
//Serial 3 rx =PA3 tx=PA2

//MPU i2c SDA = PB7 SCL = PB6

//SD card SPI CLK = PA5 MISO PA6 MOSI PA7
const uint8_t SDsel = PA4; //spi chip select for sd card
