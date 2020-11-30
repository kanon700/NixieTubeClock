/*
 * nixie_rtc.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_RTC_H_
#define INC_NIXIE_NIXIE_RTC_H_

#define CLOCK_STOP 0x20
#define CLOCK_START 0x00
#define RTC8564_ADDR  0x51 << 1
#define I2C_TIMEOUT 500
#define RTC8564_READ_ADDR 0x00
#define RTC8564_REGALL 17
#define RTC8564_REG_TIME 7

typedef enum{
  rtc_mm_addr = 0x00,
  rtc_mm_ctrl1,
  rtc_mm_ctrl2,
  rtc_mm_sec,
  rtc_mm_min,
  rtc_mm_hour,
  rtc_mm_day,
  rtc_mm_week,
  rtc_mm_month,
  rtc_mm_year,
  rtc_mm_min_a,
  rtc_mm_hour_a,
  rtc_mm_day_a,
  rtc_mm_week_a,
  rtc_mm_clk_feq,
  rtc_mm_tim_ctrl,
  rtc_mm_tim
}rtc_mm_t;

typedef struct{
  union{
    struct{
      rtc_mm_t addr;
      uint8_t ctrl1;
      uint8_t ctrl2;
      uint8_t sec;
      uint8_t min;
      uint8_t hour;
      uint8_t day;
      uint8_t week;
      uint8_t month;
      uint8_t year;
      uint8_t min_a;
      uint8_t hour_a;
      uint8_t day_a;
      uint8_t week_a;
      uint8_t clk_feq;
      uint8_t tim_ctrl;
      uint8_t tim;
    }reg;
    uint8_t regall[RTC8564_REGALL];
  };
  uint32_t time; // 時、分、秒(2桁毎)
  uint32_t date; // 年、月、日(2桁毎)
}NixieRtcHandle_t;
typedef NixieRtcHandle_t* NixieRtcHandle;

extern void nixie_rtc_init(NixieRtcHandle hrtc);
extern void nixie_rtc_ctrl(NixieRtcHandle hrtc);

#endif /* INC_NIXIE_NIXIE_RTC_H_ */
