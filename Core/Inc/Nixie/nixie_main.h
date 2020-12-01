/*
 * nixie_main.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_MAIN_H_
#define INC_NIXIE_NIXIE_MAIN_H_

#include "nixie_common.h"
#include "nixie_rtc.h"
#include "nixie_ui.h"

#define DG_RAND_TIME 2000
#define DG_SET_NUM_TIME 400

typedef enum{
  MODE_CLOCK = 0,
  MODE_CALENDAR,
  MODE_TIMER,
  MODE_SETTING
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

typedef enum{
  DRIVE_OFF = 0,
  DRIVE_12,
  DRIVE_34,
  DRIVE_56,
  DRIVE_ALL
} DRIVE_SEL_TUBE_t;

typedef enum{
  SETTING_PHASE_NONE = 0,
  SETTING_PHASE_SEC,
  SETTING_PHASE_MIN,
  SETTING_PHASE_HOUR,
  SETTING_PHASE_DAY,
  SETTING_PHASE_MONTH,
  SETTING_PHASE_YEAR,
  SETTING_PHASE_END
} SETTING_PHASE_t;

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
  struct{
    DRIVE_SEL_TUBE_t sel;
    SETTING_PHASE_t phase;
    struct{
      uint8_t sec;
      uint8_t min;
      uint8_t hour;
      uint8_t day;
      uint8_t month;
      uint8_t year;
    }time;
  } settings;
  NixieDriveTubeHandle_t tube;
  NixieDiverGenceHandle_t dg;

}NixieMainHandle_t;
typedef NixieMainHandle_t* NixieMainHandle;

extern void nixie_main_init(void);
extern void nixie_main(void);

#endif /* INC_NIXIE_NIXIE_MAIN_H_ */
