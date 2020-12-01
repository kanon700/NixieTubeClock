/*
 * nixie_common.c
 *
 *  Created on: 2020/11/27
 *      Author: kanon700
 */

#include "main.h"

uint8_t nixie_bcd2dec(uint8_t bcd){
  return (bcd/16*10) + (bcd%16);
}

uint8_t nixie_dec2bcd(uint8_t dec){
  return (dec/10*16) + (dec%10);
}
