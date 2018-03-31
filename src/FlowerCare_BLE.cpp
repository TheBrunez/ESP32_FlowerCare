#include <FlowerCare_BLE.h>

// TODO from connection to writing A0F1 to the characteristic I can't wait, so
// it's better to delete connect() and disconnect() methods and put them inside
// getData()

/*******************************************************************************
 *                                  PUBLIC
 ******************************************************************************/

/**
 * @brief Constructor
 *
 * @param addr the BLE address of the FlowerCare sensor
 */
FlowerCare::FlowerCare(std::string addr) {
  _addr = new BLEAddress(addr);
  _service_uuid = new BLEUUID(SERVICE_UUID);
  _sensorData_uuid = new BLEUUID(SENSORDATA_UUID);
  _writeMode_uuid = new BLEUUID(WRITEMODE_UUID);
  _battVers_uuid = new BLEUUID(VERSIONBATTERY_UUID);

  // initialize BLE and create client
  BLEDevice::init("");

  _BLEClient = BLEDevice::createClient();

  // initialize structure for incoming data
  _data = {};
  _plant = {};
}

/**
 * @brief Constructor
 *
 * @param addr the BLE address of the FlowerCare sensor
 * @param plant the plant type
 */
FlowerCare::FlowerCare(std::string addr, Plant plant) : FlowerCare(addr) {
  // initialize plant maximun and minimum value
  initPlant(plant);
}

/**
 * @brief Constructor
 *
 * @param addr the BLE address of the FlowerCare sensor
 * @param temp_L  the temperature level of the plant
 * @param moist_L the moisture level of the plant
 * @param light_L the light level of the plant
 * @param fert_L  the EC level of the plant
 */
FlowerCare::FlowerCare(std::string addr, Level temp_L, Level moist_L,
                       Level light_L, Level fert_L)
    : FlowerCare(addr) {
  // init plant using level
  initLevel(temp_L, moist_L, light_L, fert_L);
}

/**
 * @brief Get data from the sensor and save them in memory
 *
 * @return 0 on success, otherwise an error code is returned
 */

// TODO solve all errors, also in FlowerCare::connect()
FC_RET_T FlowerCare::getData() {
  if (_BLEClient->connect(*_addr)) {
    BLERemoteCharacteristic* pRemoteCharacteristic;

    // get FlowerCare service
    BLERemoteService* pRemoteService = _BLEClient->getService(*_service_uuid);
    if (pRemoteService == nullptr) {
      return ERR_SERVICE;
    }

    // write particular value to a characteristic to enable data reading
    // TODO errors occour during this call to getCharacteristc(). In particular
    // [E][BLERemoteCharacteristic.cpp:308] retrieveDescriptors():
    //   esp_ble_gattc_get_all_descr: ESP_GATT_NOT_FOUND
    // [E] [BLERemoteCharacteristic.cpp:315] retrieveDescriptors():
    // [E] [BLERemoteCharacteristic.cpp:315] retrieveDescriptors():

    pRemoteCharacteristic = pRemoteService->getCharacteristic(*_writeMode_uuid);
    uint8_t buf[2] = {0xA0, 0x1F};

    // TODO solve this
    /* sometimes writeValue return the following errors:
     * E (7224) BT: GATTC_Write GATT_BUSY conn_id = 3
     * E (7228) BT: No pending command
     * E (7236) BT: No pending command
     * E (15382) BT: bta_gattc_conn_cback() - cif=3 connected=0 conn_id=3
     *  reason=0x0013
     *
     * And after this the system is blocked
     */
    pRemoteCharacteristic->writeValue(buf, 2, true);

    // add small delay if necessary
    // delay(100);

    // get characteristic containing data
    pRemoteCharacteristic =
        pRemoteService->getCharacteristic(*_sensorData_uuid);

    if (pRemoteCharacteristic == nullptr) {
      return ERR_CHARACT;
    }

    // Read the value of the characteristic.
    // TODO check if reading successful
    std::string value = pRemoteCharacteristic->readValue();

    const char* val = value.c_str();

    /*
    // print HEX format of the data characteristic
    Serial.print("Hex: ");
    for (int i = 0; i < 16; i++) {
      Serial.print((int)val[i], HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
    */

    _data.temp = ((float)val[0] + val[1] * 256) / 10;
    _data.moist = val[7];
    _data.light = val[3] + val[4] * 256;
    _data.fert = val[8] + val[9] * 256;

    // disconnect
    _BLEClient->disconnect();

    return FLCARE_OK;
  }

  // unable to connect
  return ERR_CONNECT;
}

/**
 * @brief Get the last saved temperature value
 *
 * @return the last saved temperature value, in °C
 */
float FlowerCare::temp() { return _data.temp; }

/**
 * @brief Get the last saved moisture value
 *
 * @return the last saved moisture value, in %
 */
int FlowerCare::moist() { return _data.moist; }

/**
 * @brief Get the last saved light value
 *
 * @return the last saved light value, in lux
 */
int FlowerCare::light() { return _data.light; }

/**
 * @brief Get the last saved EC value
 *
 * @return  the last saved EC value, in us/cm
 */
int FlowerCare::fert() { return _data.fert; }

/**
 * @brief Update the current data structure and give the temperature value
 *
 * @return on success the current temperature value in °C, otherwise -1
 */
float FlowerCare::getTemp() { return (getData() == FLCARE_OK) ? temp() : -1; }

/**
 * @brief Update the current data structure and give the moisture value
 *
 * @return on success the current moisture value in %, otherwise -1
 */
int FlowerCare::getMoist() { return (getData() == FLCARE_OK) ? moist() : -1; }

/**
 * @brief Update the current data structure and give the light value
 *
 * @return on success the current light value in lux, otherwise -1
 */
int FlowerCare::getLight() { return (getData() == FLCARE_OK) ? light() : -1; }

/**
 * @brief Update the current data structure and give the EC value
 *
 * @return on success the current EC value in us/cm, otherwise -1
 */
int FlowerCare::getFert() { return (getData() == FLCARE_OK) ? fert() : -1; }

/**
 * @brief Check ambient temperature
 *
 * @return  0 temperature is ok
 *          1 temperature is too high
 *         -1 temperature is too low
 */
int FlowerCare::checkTemp() {
  if (_data.temp < _plant.temp_min) {
    return -1;
  } else if (_data.temp > _plant.temp_max) {
    return 1;
  }
  return 0;
}

/**
 * @brief Check soil moisture
 *
 * @return  0 moisture is ok
 *          1 moisture is too high
 *         -1 moisture is too low
 */
int FlowerCare::checkMoist() {
  if (_data.moist < _plant.moist_min) {
    return -1;
  } else if (_data.moist > _plant.moist_max) {
    return 1;
  }
  return 0;
}

/**
 * @brief Check ambient light
 *
 * @return  0 light is ok
 *          1 light is too high
 *         -1 light is too low
 */
int FlowerCare::checkLight() {
  if (_data.light < _plant.light_min) {
    return -1;
  } else if (_data.light > _plant.light_max) {
    return 1;
  }
  return 0;
}

/**
 * @brief Check soil EC
 *
 * @return  0 EC is ok
 *          1 EC is too high
 *         -1 EC is too low
 */
int FlowerCare::checkFert() {
  if (_data.fert < _plant.fert_min) {
    return -1;
  } else if (_data.fert > _plant.fert_max) {
    return 1;
  }
  return 0;
}

/*******************************************************************************
 *                                  PRIVATE
 ******************************************************************************/

/**
 * @brief Initialize plant values
 *
 * @param plant the plant name. See Plant.h for available plants
 * @return true if the plant is available
 * @return false if the plant is not available
 */
bool FlowerCare::initPlant(Plant plant) {
  switch (plant) {
    case FICUS_GINSEGN:
      _plant.moist_min = 15;
      _plant.moist_max = 30;
      _plant.light_min = 1000;
      _plant.light_max = 2000;
      _plant.temp_min = 15.0;
      _plant.temp_max = 30.0;
      _plant.fert_min = 300;
      _plant.fert_max = 600;

      return true;

    case ACALYPHA:
      initLevel(ACALYPHA_VAL);
      return true;

    case ANTHURIUM:
      initLevel(ANTHURIUM_VAL);
      return true;

    case CALADIUM:
      initLevel(CALADIUM_VAL);
      return true;

    case CALATHEA:
      initLevel(CALATHEA_VAL);
      return true;

    case CISSUS_DISCOLOR:
      initLevel(CISSUS_DISCOLOR_VAL);
      return true;

    case DIEFFENBACHIA:
      initLevel(DIEFFENBACHIA_VAL);
      return true;

    case DIZYGOTHECA:
      initLevel(DIZYGOTHECA_VAL);
      return true;

    case SAINTPAULIA:
      initLevel(SAINTPAULIA_VAL);
      return true;

    case SYNGONIUM:
      initLevel(SYNGONIUM_VAL);
      return true;

    case APHELANDRA:
      initLevel(APHELANDRA_VAL);
      return true;

    case ARAUCARIA:
      initLevel(ARAUCARIA_VAL);
      return true;

    case ASPARAGUS:
      initLevel(ASPARAGUS_VAL);
      return true;

    case BEGONIA:
      initLevel(BEGONIA_VAL);
      return true;

    case BROMELIADS:
      initLevel(BROMELIADS_VAL);
      return true;

    case CITRUS:
      initLevel(CITRUS_VAL);
      return true;

    case COLEUS:
      initLevel(COLEUS_VAL);
      return true;

    case DRACAENA:
      initLevel(DRACAENA_VAL);
      return true;

    case FERNS:
      initLevel(FERNS_VAL);
      return true;

    case FICUS:
      initLevel(FICUS_VAL);
      return true;

    case GYNURA:
      initLevel(GYNURA_VAL);
      return true;

    case HOYA:
      initLevel(HOYA_VAL);
      return true;

    case IMPATIENS:
      initLevel(IMPATIENS_VAL);
      return true;

    case KALANCHOE:
      initLevel(KALANCHOE_VAL);
      return true;

    case MARANTA:
      initLevel(MARANTA_VAL);
      return true;

    case MONSTERA:
      initLevel(MONSTERA_VAL);
      return true;

    case ORCHIDS:
      initLevel(ORCHIDS_VAL);
      return true;

    case PALM:
      initLevel(PALM_VAL);
      return true;

    case PANDANUS:
      initLevel(PANDANUS_VAL);
      return true;

    case PEPEROMIA:
      initLevel(PEPEROMIA_VAL);
      return true;

    case PHILODENDRON:
      initLevel(PHILODENDRON_VAL);
      return true;

    case SANSEVIERIA:
      initLevel(SANSEVIERIA_VAL);
      return true;

    case SCHEFFLERA:
      initLevel(SCHEFFLERA_VAL);
      return true;

    case ASPIDISTRA:
      initLevel(ASPIDISTRA_VAL);
      return true;

    case CHLOROPHYTUM:
      initLevel(CHLOROPHYTUM_VAL);
      return true;

    case CLIVIA:
      initLevel(CLIVIA_VAL);
      return true;

    case CUPHEA:
      initLevel(CUPHEA_VAL);
      return true;

    case FATSHEDERA:
      initLevel(FATSHEDERA_VAL);
      return true;

    case FATSIA:
      initLevel(FATSIA_VAL);
      return true;

    case GREVILLEA:
      initLevel(GREVILLEA_VAL);
      return true;

    case HEDERA:
      initLevel(HEDERA_VAL);
      return true;

    case HELXINE:
      initLevel(HELXINE_VAL);
      return true;

    case LAURUS:
      initLevel(LAURUS_VAL);
      return true;

    case PELARGONIUM:
      initLevel(PELARGONIUM_VAL);
      return true;

    case SAXIFRAGA:
      initLevel(SAXIFRAGA_VAL);
      return true;

    case SUCCULENTS:
      initLevel(SUCCULENTS_VAL);
      return true;

    case TRADESCANTIA:
      initLevel(TRADESCANTIA_VAL);
      return true;

    case VINES:
      initLevel(VINES_VAL);
      return true;

    case YUCCA:
      initLevel(YUCCA_VAL);
      return true;

    case ROSMARINUS_OFFICINALIS:
      initLevel(ROSMARINUS_OFFICINALIS_VAL);
      return true;

    case THYMUS_VULGARIS:
      initLevel(THYMUS_VULGARIS_VAL);
      return true;

    case SALVIA_OFFICINALIS_LATIFOLIA:
      initLevel(SALVIA_OFFICINALIS_LATIFOLIA_VAL);
      return true;

    case OCIMUM_BASILICUM:
      initLevel(OCIMUM_BASILICUM_VAL);
      return true;

    default:
      return false;
  }
}

/**
 * @brief Initialize plant values basing on the specified level.
 * Levels can be _LOW, _MED, _HIGH or _ND not available data.
 * In _ND case _MED values will be used
 *
 * @param temp_L    temperature level
 * @param moist_L   moisture level
 * @param light_L   light level
 * @param fert_L    soil EC level
 */
void FlowerCare::initLevel(Level temp_L, Level moist_L, Level light_L,
                           Level fert_L) {
  // temp in °C
  if (temp_L == _LOW) {
    _plant.temp_min = _LOW_TEMPMIN;
    _plant.temp_max = _LOW_TEMPMAX;
  } else if (temp_L == _HIGH) {
    _plant.temp_min = _HIGH_TEMPMIN;
    _plant.temp_max = _HIGH_TEMPMAX;
  } else {
    _plant.temp_min = _MED_TEMPMIN;
    _plant.temp_max = _MED_TEMPMAX;
  }

  // moisture in %
  if (moist_L == _LOW) {
    _plant.moist_min = _LOW_MOISTMIN;
    _plant.moist_max = _LOW_MOISTMAX;
  } else if (moist_L == _HIGH) {
    _plant.moist_min = _HIGH_MOISTMIN;
    _plant.moist_max = _HIGH_MOISTMAX;
  } else {
    _plant.moist_min = _MED_MOISTMIN;
    _plant.moist_max = _MED_MOISTMAX;
  }

  // light in lux
  if (light_L == _LOW) {
    _plant.light_min = _LOW_LIGHTMIN;
    _plant.light_max = _LOW_LIGHTMAX;
  } else if (light_L == _HIGH) {
    _plant.light_min = _HIGH_LIGHTMIN;
    _plant.light_max = _HIGH_LIGHTMAX;
  } else {
    _plant.light_min = _MED_LIGHTMIN;
    _plant.light_max = _MED_LIGHTMAX;
  }

  // fert in us/cm
  if (fert_L == _LOW) {
    _plant.fert_min = _LOW_FERTMIN;
    _plant.fert_max = _LOW_FERTMAX;
  } else if (fert_L == _HIGH) {
    _plant.fert_min = _HIGH_FERTMIN;
    _plant.fert_max = _HIGH_FERTMAX;
  } else {
    _plant.fert_min = _MED_FERTMIN;
    _plant.fert_max = _MED_FERTMAX;
  }
}
