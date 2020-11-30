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
      uint8_t no1;
      uint8_t no2;
      uint8_t no3;
      uint8_t no4;
      uint8_t no5;
      uint8_t no6;
    }tube;
    uint8_t tubeall[TUBE_ALL];
  };
}NixieDriverHandle_t;
typedef NixieDriverHandle_t* NixieDriverHandle;

extern void nixie_driver_task(uint32_t num);

#endif /* INC_NIXIE_NIXIE_DRIVER_H_ */
