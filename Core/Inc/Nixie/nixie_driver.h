/*
 * nixie_driver.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_DRIVER_H_
#define INC_NIXIE_NIXIE_DRIVER_H_

#include "nixie_common.h"

#define NIXIE_ON_TIME 2
#define NIXIE_OFF_TIME 1

typedef enum {
  PHASE_ON = 0,
  PHASE_OFF
} DRIVER_PHASE;

typedef struct{
  DRIVER_PHASE phase;
  TUBU_NUM_t sel_tube;
  uint32_t last_time;
  union{
    struct{
      uint8_t tube1_num;
      uint8_t tube2_num;
      uint8_t tube3_num;
      uint8_t tube4_num;
      uint8_t tube5_num;
      uint8_t tube6_num;
    }tube;
    uint8_t tubeall[TUBE_ALL];
  };
}NixieDriverHandle_t;
typedef NixieDriverHandle_t* NixieDriverHandle;

extern void nixie_driver_task(uint32_t num, NixieDriveTubeHandle_t tube);

#endif /* INC_NIXIE_NIXIE_DRIVER_H_ */
