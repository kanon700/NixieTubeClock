/*
 * nixie_common.c
 *
 *  Created on: 2020/11/27
 *      Author: kanon700
 */

#include "main.h"

// 2桁のBCDを２桁のDECに変換する
uint8_t nixie_bcd2dec(uint8_t bcd){
  uint8_t a1,a2;

  a1 = bcd & 0x0f;
  a2 = (bcd >> 4) & 0x0f;

  return a2 * 10 + a1;
}
