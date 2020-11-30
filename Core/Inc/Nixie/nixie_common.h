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


extern uint8_t nixie_bcd2dec(uint8_t bcd);

#endif /* INC_NIXIE_NIXIE_COMMON_H_ */
