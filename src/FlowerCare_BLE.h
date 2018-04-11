#ifndef FLOWERCARE_BLE_H
#define FLOWERCARE_BLE_H

// WORKING WITH FLOWER CARE FIRMWARE V3.1.8

/* TODO
 * battery check
 * Temperatute - tenth of degree
 */

/* two errors happens: during connection btc_gattc_call_handler()
 * and after getting data bta_gattc_conn_cback()
 * - cif=3 connected=0 conn_id=3 reason=0x0016
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include "Plants.h"

/* SERVICES FOUND
0000fe95-0000-1000-8000-00805f9b34fb
*/

// UUID for BLE, do some research
#define SERVICE_UUID "00001204-0000-1000-8000-00805f9b34fb"
#define SENSORDATA_UUID "00001a01-0000-1000-8000-00805f9b34fb"
#define WRITEMODE_UUID "00001a00-0000-1000-8000-00805f9b34fb"
#define VERSIONBATTERY_UUID "00001a02-0000-1000-8000-00805f9b34fb"

/**
 * @brief Error code
 *
 */
enum FC_RET_T {
  FLCARE_OK = 0,
  ERR_CONNECT,      // connection error
  ERR_ALREADYCONN,  // already connected
  ERR_NOCONN,       // no connection
  ERR_SERVICE,      // serviceUUID not found
  ERR_CHARACT,      // characteristicUUID not found
};

/**
 * @brief Struct containing plant values
 *
 */
typedef struct PlantVal {
  float temp_max, temp_min;
  int moist_max, moist_min, light_max, light_min, fert_max, fert_min;
} PlantVal_t;

/**
 * @brief Struct used to hold all data from a sensor
 *
 */
typedef struct FlowerCareData {
  float temp;
  int moist, light, fert;
} FlowerCareData_t;

class FlowerCare {
 public:
  FlowerCare(std::string);
  FlowerCare(std::string, Plant);
  FlowerCare(std::string, Level, Level, Level, Level);

  FC_RET_T getData(FlowerCareData_t* = NULL);
  float temp();
  int moist();
  int light();
  int fert();

  float getTemp();
  int getMoist();
  int getLight();
  int getFert();

  int checkTemp();
  int checkMoist();
  int checkLight();
  int checkFert();

 private:
  BLEAddress* _addr; /**< BLE address of Flower Care sensor */
  BLEUUID *_service_uuid, *_sensorData_uuid, *_writeMode_uuid, *_battVers_uuid;
  FlowerCareData_t _data; /**< Struct to hold Flower Care data */
  PlantVal_t _plant;      /**< Struct to hold plant values */
  BLEClient* _BLEClient;  /**< BLE client */

  bool initPlant(Plant);
  void initLevel(Level, Level, Level, Level);
};

#endif
