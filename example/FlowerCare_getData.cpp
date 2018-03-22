/*******************************************************************************
 * In this example we request the data from the sensor and then print them on
 * the serial monitor
 * The data is requested about every 10 minutes
 ******************************************************************************/
#include <FlowerCare_BLE.h>

// sensor address
#define FLORA_ADDR "XX:XX:XX:XX:XX:XX"
// plant type. See Plant.h for more plants
#define PLANT FICUS_GINSEGN

// 10 minutes in seconds
#define TEN_MINUTES 600000

FlowerCare* flora;

// Variable to store functions return value
FC_RET_T ret;

void setup() {
  flora = new FlowerCare(FLORA_ADDR, PLANT);

  Serial.begin(9600);
}

void loop() {
  // Connect to sensor
  Serial.print("Connecting to sensor");
  ret = flora->connect();
  while (ret != FLCARE_OK) {
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // request data from sensor
  Serial.print("Getting data from sensor");
  ret = flora->getData();
  while (ret != FLCARE_OK) {
    Serial.print(".");
  }
  Serial.println("\nData acquired!");

  // print data
  Serial.print("Ambient temperature: ");
  Serial.print(flora->temp());
  Serial.println("°C");
  Serial.print("Soil moisture: ");
  Serial.print(flora->moist());
  Serial.println("%");
  Serial.print("Ambient light: ");
  Serial.print(flora->light());
  Serial.println("lux");
  Serial.print("Soil EC: ");
  Serial.print(flora->fert());
  Serial.println("us/cm");

  // disconnect from sensor
  flora->disconnect();

  delay(TEN_MINUTES);
}
