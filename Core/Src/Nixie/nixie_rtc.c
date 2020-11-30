/*
 * nixie_rtc.c
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include "i2c.h"
#include "nixie_rtc.h"
#include "nixie_common.h"

void rtc_read_time(NixieRtcHandle hrtc);
void rtc_set_time(NixieRtcHandle hrtc);
void rtc_write_single_data(NixieRtcHandle hrtc, rtc_mm_t addr, uint8_t data);
void rtc_write_all_time(NixieRtcHandle hrtc);
void rtc_write_all_reg(NixieRtcHandle hrtc);
void rtc_data_mask(NixieRtcHandle hrtc);
HAL_StatusTypeDef rtc_check_vl(uint8_t reg);

void nixie_rtc_init(NixieRtcHandle hrtc){
  hrtc->reg.addr = rtc_mm_ctrl2;
  HAL_I2C_Master_Transmit(&hi2c1, RTC8564_ADDR, &hrtc->reg.addr, 1, I2C_TIMEOUT);
  HAL_I2C_Master_Receive(&hi2c1, RTC8564_ADDR, &hrtc->reg.sec, 1, I2C_TIMEOUT);

  if(rtc_check_vl(hrtc->reg.sec) == HAL_ERROR){
    hrtc->reg.sec = 0x00;
    rtc_write_all_reg(hrtc);
    rtc_write_single_data(hrtc, rtc_mm_ctrl1, CLOCK_START);
  }
}

void nixie_rtc_ctrl(NixieRtcHandle hrtc){
  rtc_read_time(hrtc);
}

void rtc_read_time(NixieRtcHandle hrtc){
  hrtc->reg.addr = rtc_mm_ctrl2;
  HAL_I2C_Master_Transmit(&hi2c1, RTC8564_ADDR, &hrtc->reg.addr, 1, I2C_TIMEOUT);
  HAL_I2C_Master_Receive(&hi2c1, RTC8564_ADDR, &hrtc->reg.sec, RTC8564_REG_TIME, I2C_TIMEOUT);
  rtc_data_mask(hrtc);
  hrtc->time = (nixie_bcd2dec(hrtc->reg.hour) * 10000) + (nixie_bcd2dec(hrtc->reg.min)   * 100) + nixie_bcd2dec(hrtc->reg.sec);
  hrtc->date = (nixie_bcd2dec(hrtc->reg.year) * 10000) + (nixie_bcd2dec(hrtc->reg.month) * 100) + nixie_bcd2dec(hrtc->reg.day);
}

void rtc_set_time(NixieRtcHandle hrtc){
  rtc_write_single_data(hrtc, rtc_mm_ctrl1, CLOCK_STOP);
  rtc_write_all_time(hrtc);
  rtc_write_single_data(hrtc, rtc_mm_ctrl1, CLOCK_START);
}

void rtc_write_single_data(NixieRtcHandle hrtc, rtc_mm_t addr, uint8_t data){
  hrtc->reg.addr = addr;
  hrtc->reg.ctrl1 = data;
  HAL_I2C_Master_Transmit(&hi2c1, RTC8564_ADDR, &hrtc->reg.addr, 2, I2C_TIMEOUT);
}

void rtc_write_all_time(NixieRtcHandle hrtc){
  hrtc->reg.addr = rtc_mm_sec;
  HAL_I2C_Master_Transmit(&hi2c1, RTC8564_ADDR, &hrtc->reg.sec, RTC8564_REG_TIME, I2C_TIMEOUT);
}

void rtc_write_all_reg(NixieRtcHandle hrtc){
  hrtc->reg.addr = rtc_mm_addr;
  HAL_I2C_Master_Transmit(&hi2c1, RTC8564_ADDR, hrtc->regall, RTC8564_REGALL, I2C_TIMEOUT);
}

// RTCの不定データをマスクする
void rtc_data_mask(NixieRtcHandle hrtc){
  hrtc->reg.min   &= 0x7F;
  hrtc->reg.hour  &= 0x3F;
  hrtc->reg.day   &= 0x3F;
  hrtc->reg.week  &= 0x07;
  hrtc->reg.month &= 0x1F;
  hrtc->reg.year  &= 0xFF;
}

HAL_StatusTypeDef rtc_check_vl(uint8_t reg){
  if(reg & 0x80){
    return HAL_ERROR;
  }
  else {
    return HAL_OK;
  }
}
