/*
 * nixie_main.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_MAIN_H_
#define INC_NIXIE_NIXIE_MAIN_H_

#include "nixie_rtc.h"
#include "nixie_ui.h"

#define DG_RAND_TIME 2000
#define DG_SET_NUM_TIME 400

typedef enum{
  MODE_CLOCK = 0,
  MODE_CALENDAR,
  MODE_TIMER,
  MODE_SETTING,
  MODE_OFF
}NIXIE_MODE_t;

typedef enum{
  TIMER_STOP = 0,
  TIMER_START,
}NIXIE_TIMER_t;

typedef enum{
  PHASE_NONE = 0,
  PHASE_ALL_RAND,
  PHASE_SET4,
  PHASE_SET2,
  PHASE_SET6,
  PHASE_SET3,
  PHASE_SET5,
  PHASE_SET1,
} DG_PHASE_t;

typedef struct{
  uint8_t tube[6];
  uint32_t disp;
  DG_PHASE_t phase;
  uint32_t timer;
  uint32_t change_num_timer;
}NixieDiverGenceHandle_t;
typedef NixieDiverGenceHandle_t* NixieDiverGenceHandle;

typedef struct{
  uint32_t display_num;
  struct{
    uint8_t led;
    uint8_t illumination;
  } adc;
  NIXIE_MODE_t mode;
  NixieRtcHandle_t hrtc;
  NixieUiHandle_t hui;
  struct{
    NIXIE_TIMER_t state;
    uint32_t start;
    uint32_t stop;
    uint32_t now;
    uint32_t time;
  } timer;
  NixieDiverGenceHandle_t dg;

}NixieMainHandle_t;
typedef NixieMainHandle_t* NixieMainHandle;

extern void nixie_main_init(void);
extern void nixie_main(void);

#endif /* INC_NIXIE_NIXIE_MAIN_H_ */
