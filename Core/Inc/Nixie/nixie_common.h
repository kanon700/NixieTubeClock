/*
 * nixie_common.h
 *
 *  Created on: 2020/11/27
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_COMMON_H_
#define INC_NIXIE_NIXIE_COMMON_H_

typedef enum {
  TUBE_1 = 0,
  TUBE_2,
  TUBE_3,
  TUBE_4,
  TUBE_5,
  TUBE_6,
  TUBE_ALL
} TUBU_NUM_t;

typedef enum{
  NIXIE_DRIVE_OFF = 0,
  NIXIE_DRIVE_ON
}NIXIE_DRIVE_t;

typedef union{
  struct{
    NIXIE_DRIVE_t tube1;
    NIXIE_DRIVE_t tube2;
    NIXIE_DRIVE_t tube3;
    NIXIE_DRIVE_t tube4;
    NIXIE_DRIVE_t tube5;
    NIXIE_DRIVE_t tube6;
  } drive;
  NIXIE_DRIVE_t all[TUBE_ALL];
} NixieDriveTubeHandle_t;
typedef NixieDriveTubeHandle_t* NixieDriveTubeHandle;

extern uint8_t nixie_bcd2dec(uint8_t bcd);
extern uint8_t nixie_dec2bcd(uint8_t dec);

#endif /* INC_NIXIE_NIXIE_COMMON_H_ */
