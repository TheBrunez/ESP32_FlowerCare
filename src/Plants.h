#ifndef PLANTS_H
#define PLANTS_H

/* TODO implement _MEDHIGH and _MEDLOW levels if necessary
 * Implement light control over the day (integral of lux over day)
 */

// temperature levels in °C
#define _LOW_TEMPMIN 5
#define _LOW_TEMPMAX 15
#define _MED_TEMPMIN 10
#define _MED_TEMPMAX 24
#define _HIGH_TEMPMIN 15
#define _HIGH_TEMPMAX 30

// moisture levels in %
#define _LOW_MOISTMIN 10
#define _LOW_MOISTMAX 30
#define _MED_MOISTMIN 30
#define _MED_MOISTMAX 50
#define _HIGH_MOISTMIN 40
#define _HIGH_MOISTMAX 60

// light levels in lux
#define _LOW_LIGHTMIN 200
#define _LOW_LIGHTMAX 4000
#define _MED_LIGHTMIN 4000
#define _MED_LIGHTMAX 20000
#define _HIGH_LIGHTMIN 15000
#define _HIGH_LIGHTMAX 50000

// EC level in us/cm
#define _LOW_FERTMIN 100
#define _LOW_FERTMAX 300
#define _MED_FERTMIN 300
#define _MED_FERTMAX 600
#define _HIGH_FERTMIN 600
#define _HIGH_FERTMAX 900

enum Level {
  _HIGH,
  _MED,
  _LOW,
  _ND,  // data not available
};

enum Plant {
  // test plant
  FICUS_GINSEGN,
  // high temperature indoor plants (tender plants) www.coolgarden.me
  ACALYPHA,
  ANTHURIUM,
  CALADIUM,
  CALATHEA,
  CISSUS_DISCOLOR,
  DIEFFENBACHIA,
  DIZYGOTHECA,
  SAINTPAULIA,
  SYNGONIUM,
  // med temperature indoor plants (non-hardy plants) www.coolgarden.me
  APHELANDRA,
  ARAUCARIA,
  ASPARAGUS,
  BEGONIA,
  BROMELIADS,
  CITRUS,
  COLEUS,
  DRACAENA,
  FERNS,
  FICUS,
  GYNURA,
  HOYA,
  IMPATIENS,
  KALANCHOE,
  MARANTA,
  MONSTERA,
  ORCHIDS,
  PALM,
  PANDANUS,
  PEPEROMIA,
  PHILODENDRON,
  SANSEVIERIA,
  SCHEFFLERA,
  // low temperature indoor plants (hardy plants) www.coolgarden.me
  ASPIDISTRA,
  CHLOROPHYTUM,
  CLIVIA,
  CUPHEA,
  FATSHEDERA,
  FATSIA,
  GREVILLEA,
  HEDERA,
  HELXINE,
  LAURUS,
  PELARGONIUM,
  SAXIFRAGA,
  SUCCULENTS,
  TRADESCANTIA,
  VINES,
  YUCCA,
};

/* #define [plantName]_VAL [temp Level],[moist Level],[light Level],
                           [fert Level]
*/
// high temperature indoor plants (tender plants) www.coolgarden.me
#define ACALYPHA_VAL _LOW, _ND, _ND, _ND
#define ANTHURIUM_VAL _LOW, _ND, _ND, _ND
#define CALADIUM_VAL _LOW, _ND, _ND, _ND
#define CALATHEA_VAL _LOW, _ND, _ND, _ND
#define CISSUS_DISCOLOR_VAL _LOW, _ND, _ND, _ND
#define DIEFFENBACHIA_VAL _LOW, _ND, _ND, _ND
#define DIZYGOTHECA_VAL _LOW, _ND, _ND, _ND
#define SAINTPAULIA_VAL _LOW, _ND, _ND, _ND
#define SYNGONIUM_VAL _LOW, _ND, _ND, _ND
// med temperature indoor plants (non-hardy plants) www.coolgarden.me
#define APHELANDRA_VAL _MED, _ND, _ND, _ND
#define ARAUCARIA_VAL _MED, _ND, _ND, _ND
#define ASPARAGUS_VAL _MED, _ND, _ND, _ND
#define BEGONIA_VAL _MED, _ND, _ND, _ND
#define BROMELIADS_VAL _MED, _ND, _ND, _ND
#define CITRUS_VAL _MED, _ND, _ND, _ND
#define COLEUS_VAL _MED, _ND, _ND, _ND
#define DRACAENA_VAL _MED, _ND, _ND, _ND
#define FERNS_VAL _MED, _ND, _ND, _ND
#define FICUS_VAL _MED, _ND, _ND, _ND
#define GYNURA_VAL _MED, _ND, _ND, _ND
#define HOYA_VAL _MED, _ND, _ND, _ND
#define IMPATIENS_VAL _MED, _ND, _ND, _ND
#define KALANCHOE_VAL _MED, _ND, _ND, _ND
#define MARANTA_VAL _MED, _ND, _ND, _ND
#define MONSTERA_VAL _MED, _ND, _ND, _ND
#define ORCHIDS_VAL _MED, _ND, _ND, _ND
#define PALM_VAL _MED, _ND, _ND, _ND
#define PANDANUS_VAL _MED, _ND, _ND, _ND
#define PEPEROMIA_VAL _MED, _ND, _ND, _ND
#define PHILODENDRON_VAL _MED, _ND, _ND, _ND
#define SANSEVIERIA_VAL _MED, _ND, _ND, _ND
#define SCHEFFLERA_VAL _MED, _ND, _ND, _ND
// low temperature indoor plants (hardy plants) www.coolgarden.me
#define ASPIDISTRA_VAL _HIGH, _ND, _ND, _ND
#define CHLOROPHYTUM_VAL _HIGH, _ND, _ND, _ND
#define CLIVIA_VAL _HIGH, _ND, _ND, _ND
#define CUPHEA_VAL _HIGH, _ND, _ND, _ND
#define FATSHEDERA_VAL _HIGH, _ND, _ND, _ND
#define FATSIA_VAL _HIGH, _ND, _ND, _ND
#define GREVILLEA_VAL _HIGH, _ND, _ND, _ND
#define HEDERA_VAL _HIGH, _ND, _ND, _ND
#define HELXINE_VAL _HIGH, _ND, _ND, _ND
#define LAURUS_VAL _HIGH, _ND, _ND, _ND
#define PELARGONIUM_VAL _HIGH, _ND, _ND, _ND
#define SAXIFRAGA_VAL _HIGH, _ND, _ND, _ND
#define SUCCULENTS_VAL _HIGH, _ND, _ND, _ND
#define TRADESCANTIA_VAL _HIGH, _ND, _ND, _ND
#define VINES_VAL _HIGH, _ND, _ND, _ND
#define YUCCA_VAL _HIGH, _ND, _ND, _ND

//piante messe da piergiorgio
/* #define [plantName]_VAL [temperatura],[acqua],[luce],
                           [fertilità]
*/

#define ROSMARINUS_OFFICINALIS _LOW, _MED, _HIGH, _LOW

#endif