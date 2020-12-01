/*
 * nixie_main.c
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include <stdlib.h>
#include "main.h"
#include "nixie_main.h"
#include "nixie_common.h"
#include "nixie_ui.h"
#include "nixie_led.h"
#include "nixie_adc.h"
#include "nixie_driver.h"
#include "nixie_rtc.h"

#define CALC_TIME(a) (a / 1000 / 60) * 10000 + (a / 1000 % 60) * 100 + (a % 1000 / 10)
#define DIVERGENCE_START_SEC 55


NixieMainHandle_t hmain = {
    .settings.sel = DRIVE_12
};

NixieRtcHandle_t hrtc = {
    .reg.ctrl1    = 0x20,
    .reg.ctrl2    = 0x00,
    .reg.sec      = 0x00,
    .reg.min      = 0x46,
    .reg.hour     = 0x21,
    .reg.day      = 0x18,
    .reg.week     = 0x03,
    .reg.month    = 0x11,
    .reg.year     = 0x20,
    .reg.min_a    = 0x80,
    .reg.hour_a   = 0x80,
    .reg.day_a    = 0x80,
    .reg.week_a   = 0x80,
    .reg.clk_feq  = 0x00,
    .reg.tim_ctrl = 0x00,
    .reg.tim      = 0x00
};

void nixie_set_mode(void);
void nixie_set_display_num(void);
void nixie_timer(void);
void nixie_setting(void);
void nixie_display_toggle(void);
void nixie_set_time(void);
void nixie_drive_tube_select(DRIVE_SEL_TUBE_t sel);
void nixie_display_off_by_illumination(void);

void setting_time(void);
void setting_get_time(void);
void setting_set_time(void);
void setting_set_display_num(void);

void dg_judge(void);
void dg_set_display_num(void);
void dg_ctrl_timming(void);
void dg_set_tube2disp(void);

void nixie_main_init(void){
  hmain.hrtc = hrtc;
  nixie_led_init();
  nixie_rtc_init(&hrtc);
}

void nixie_main(void){
  nixie_adc_exec();
  nixie_get_illumination(&hmain.adc.illumination);
  nixie_update_led(hmain.adc.illumination);
  nixie_set_mode();
  nixie_set_display_num();
  nixie_driver_task(hmain.display_num, hmain.tube);
  nixie_rtc_ctrl(&hmain.hrtc);
}

void HAL_SYSTICK_Callback(void){
  nixie_ui_btn_callback();
}

void nixie_set_mode(void){
  uint8_t btn = nixie_ui_btn_get_event();

  if(btn & NIXIE_UI_BTN_FORWARD){
    if(hmain.mode == MODE_SETTING){
      hmain.mode = MODE_CLOCK;
    }
    else{
      hmain.mode++;
    }
    nixie_ui_btn_clear_event();
  }
}

void nixie_set_display_num(void){
  switch(hmain.mode){
  case MODE_CLOCK:
    nixie_display_off_by_illumination();
    nixie_set_time();
    break;
  case MODE_CALENDAR:
    nixie_display_off_by_illumination();
    hmain.display_num = hmain.hrtc.date;
    break;
  case MODE_TIMER:
    nixie_display_off_by_illumination();
    nixie_timer();
    break;
  case MODE_SETTING:
    nixie_setting();
    break;
  default:
    break;
  }
}

void nixie_timer(void){
  uint8_t btn = nixie_ui_btn_get_event();

  if(btn & NIXIE_UI_BTN_ENTER){
    if(hmain.timer.state == TIMER_STOP){
      hmain.timer.state = TIMER_START;
      hmain.timer.start = HAL_GetTick();
    }
    else{
      hmain.timer.state = TIMER_STOP;
      hmain.timer.stop = hmain.timer.now;
    }
    nixie_ui_btn_clear_event();
  }
  if(btn & NIXIE_UI_BTN_BACKWARD){
    hmain.timer.start = HAL_GetTick();
    hmain.timer.stop = 0;
    hmain.timer.time = 0;
    nixie_ui_btn_clear_event();
  }

  if(hmain.timer.state == TIMER_START){
    hmain.timer.now = (HAL_GetTick() - hmain.timer.start) + hmain.timer.stop;
    hmain.timer.time = CALC_TIME(hmain.timer.now);
  }
  hmain.display_num = hmain.timer.time;
}

void nixie_setting(void){
  uint32_t toggle_timing = 0;

  uint8_t btn = nixie_ui_btn_get_event();
  if(btn & NIXIE_UI_BTN_ENTER){
    nixie_ui_btn_clear_event();
    hmain.settings.phase++;
    setting_get_time();

    while(1){
      // 1s毎にトグル表示
      if(HAL_GetTick() - toggle_timing > 500){
        toggle_timing = HAL_GetTick();
        nixie_display_toggle();
      }

      // フェーズ更新
      uint8_t btn = nixie_ui_btn_get_event();
      if(btn & NIXIE_UI_BTN_ENTER){
        nixie_ui_btn_clear_event();
        hmain.settings.phase++;
        if(hmain.settings.phase == SETTING_PHASE_END){
          break;
        }
      }
      // 時刻設定
      setting_time();
      setting_set_display_num();
      nixie_driver_task(hmain.display_num, hmain.tube);
    }
    // RTC書き込み
    hmain.settings.phase = SETTING_PHASE_NONE;
    hmain.mode = MODE_CLOCK;
    setting_set_time();
    nixie_rtc_set_time(&hmain.hrtc);
  }
  else{
    hmain.display_num = hmain.adc.illumination;
  }
}

void setting_get_time(void){
  NixieRtcHandle hrtc = &hmain.hrtc;
  hmain.settings.time.sec = 0;
  hmain.settings.time.min = nixie_bcd2dec(hrtc->reg.min);
  hmain.settings.time.hour = nixie_bcd2dec(hrtc->reg.hour);
  hmain.settings.time.day = nixie_bcd2dec(hrtc->reg.day);
  hmain.settings.time.month = nixie_bcd2dec(hrtc->reg.month);
  hmain.settings.time.year = nixie_bcd2dec(hrtc->reg.year);
}

void setting_set_time(void){
  NixieRtcHandle hrtc = &hmain.hrtc;
  hrtc->reg.sec = nixie_dec2bcd(hmain.settings.time.sec);
  hrtc->reg.min = nixie_dec2bcd(hmain.settings.time.min);
  hrtc->reg.hour = nixie_dec2bcd(hmain.settings.time.hour);
  hrtc->reg.day = nixie_dec2bcd(hmain.settings.time.day);
  hrtc->reg.month = nixie_dec2bcd(hmain.settings.time.month);
  hrtc->reg.year = nixie_dec2bcd(hmain.settings.time.year);
}

void setting_set_display_num(void){
  switch(hmain.settings.phase){
  case SETTING_PHASE_SEC:
  case SETTING_PHASE_MIN:
  case SETTING_PHASE_HOUR:
    hmain.display_num = (hmain.settings.time.hour * 10000) +(hmain.settings.time.min * 100) + hmain.settings.time.sec;
    break;
  case SETTING_PHASE_DAY:
  case SETTING_PHASE_MONTH:
  case SETTING_PHASE_YEAR:
    hmain.display_num = (hmain.settings.time.year * 10000) +(hmain.settings.time.month * 100) + hmain.settings.time.day;
    break;
  case SETTING_PHASE_NONE:
  case SETTING_PHASE_END:
  default:
    hmain.display_num = 0;
  }
}

void setting_time(void){
  uint8_t btn = nixie_ui_btn_get_event();

  switch(hmain.settings.phase){
  case SETTING_PHASE_SEC:
    hmain.settings.sel = DRIVE_12;
    if(btn & NIXIE_UI_BTN_FORWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.sec == 0){
        hmain.settings.time.sec = 59;
      }
      else{
        hmain.settings.time.sec--;
      }
    }
    if(btn & NIXIE_UI_BTN_BACKWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.sec == 59){
        hmain.settings.time.sec = 0;
      }
      else{
        hmain.settings.time.sec++;
      }
    }
    break;
  case SETTING_PHASE_MIN:
    hmain.settings.sel = DRIVE_34;
    if(btn & NIXIE_UI_BTN_FORWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.min == 0){
        hmain.settings.time.min = 59;
      }
      else{
        hmain.settings.time.min--;
      }
    }
    if(btn & NIXIE_UI_BTN_BACKWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.min == 59){
        hmain.settings.time.min = 0;
      }
      else{
        hmain.settings.time.min++;
      }
    }
    break;
  case SETTING_PHASE_HOUR:
    hmain.settings.sel = DRIVE_56;
    if(btn & NIXIE_UI_BTN_FORWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.hour == 0){
        hmain.settings.time.hour = 23;
      }
      else{
        hmain.settings.time.hour--;
      }
    }
    if(btn & NIXIE_UI_BTN_BACKWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.hour == 23){
        hmain.settings.time.hour = 0;
      }
      else{
        hmain.settings.time.hour++;
      }
    }
    break;
  case SETTING_PHASE_DAY:
    hmain.settings.sel = DRIVE_12;
    if(btn & NIXIE_UI_BTN_FORWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.day == 1){
        hmain.settings.time.day = 31;
      }
      else{
        hmain.settings.time.day--;
      }
    }
    if(btn & NIXIE_UI_BTN_BACKWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.day == 31){
        hmain.settings.time.day = 1;
      }
      else{
        hmain.settings.time.day++;
      }
    }
    break;
  case SETTING_PHASE_MONTH:
    hmain.settings.sel = DRIVE_34;
    if(btn & NIXIE_UI_BTN_FORWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.month == 1){
        hmain.settings.time.month = 12;
      }
      else{
        hmain.settings.time.month--;
      }
    }
    if(btn & NIXIE_UI_BTN_BACKWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.month == 12){
        hmain.settings.time.month = 1;
      }
      else{
        hmain.settings.time.month++;
      }
    }
    break;
  case SETTING_PHASE_YEAR:
    hmain.settings.sel = DRIVE_56;
    if(btn & NIXIE_UI_BTN_FORWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.year == 0){
        hmain.settings.time.year = 99;
      }
      else{
        hmain.settings.time.year--;
      }
    }
    if(btn & NIXIE_UI_BTN_BACKWARD){
      nixie_ui_btn_clear_event();
      if(hmain.settings.time.year == 99){
        hmain.settings.time.year = 0;
      }
      else{
        hmain.settings.time.year++;
      }
    }
    break;
  case SETTING_PHASE_NONE:
  case SETTING_PHASE_END:
  default:
    break;
  }
}

void nixie_display_toggle(void){
  static uint8_t ill = 0;
  if(ill == 0){
    ill = 10;
  }
  else{
    ill = 0;
  }
  nixie_update_led(ill);
  if(ill == 0){
    nixie_drive_tube_select(hmain.settings.sel);
  }
  else{
    nixie_drive_tube_select(DRIVE_ALL);
  }
}

void nixie_set_time(void){
  NixieDiverGenceHandle dg = &hmain.dg;
  dg_judge();
  if(dg->phase != PHASE_NONE){
    dg_ctrl_timming();
    dg_set_display_num();
  }
  else{
    hmain.display_num = hmain.hrtc.time;
  }

}

void dg_judge(void){
  NixieDiverGenceHandle dg = &hmain.dg;
  if(nixie_bcd2dec(hmain.hrtc.reg.sec) < DIVERGENCE_START_SEC){
    dg->phase = PHASE_NONE;
  }
  else if((nixie_bcd2dec(hmain.hrtc.reg.sec) > DIVERGENCE_START_SEC) &&
      (dg->phase == PHASE_NONE)){
    dg->timer = HAL_GetTick();
    dg->phase++;
  }

}

void dg_ctrl_timming(void){
  NixieDiverGenceHandle dg = &hmain.dg;
  uint32_t judge_time = DG_SET_NUM_TIME;

  if(dg->phase == PHASE_ALL_RAND){
    judge_time = DG_RAND_TIME;
  }

  switch(dg->phase){
  case PHASE_ALL_RAND:
  case PHASE_SET4:
  case PHASE_SET2:
  case PHASE_SET6:
  case PHASE_SET3:
  case PHASE_SET5:
  case PHASE_SET1:
    if(HAL_GetTick() - dg->timer > judge_time){
      dg->timer = HAL_GetTick();
      dg->phase++;
    }
    break;
  case PHASE_NONE:
  default:
    break;
  }
}

void dg_set_display_num(void){
  NixieDiverGenceHandle dg = &hmain.dg;
  uint32_t sec = 0;
  uint32_t min = nixie_bcd2dec(hmain.hrtc.reg.min);
  uint32_t hour = nixie_bcd2dec(hmain.hrtc.reg.hour);

  // 切り替わり後の時間を設定する
  if(min == 59){
    min = 0;
    if(hour == 23){
      hour = 0;
    }
    else{
      hour++;
    }
  }
  else{
    min++;
  }

  // 一定時間毎にランダム数字変化させる。PHASE毎に数字を確定していく
  if(HAL_GetTick() - dg->change_num_timer > 15){
    dg->change_num_timer = HAL_GetTick();

    for(int i=0; i<6; i++){
      dg->tube[i] = (uint8_t)(rand() % 9);
    }

    switch(dg->phase){
    case PHASE_SET4:
      dg->tube[TUBE_4] = (min % 100) / 10;
      break;
    case PHASE_SET2:
      dg->tube[TUBE_2] = (sec % 100) / 10;
      dg->tube[TUBE_4] = (min % 100) / 10;
      break;
    case PHASE_SET6:
      dg->tube[TUBE_2] = (sec % 100) / 10;
      dg->tube[TUBE_4] = (min % 100) / 10;
      dg->tube[TUBE_6] = (hour % 100) / 10;
      break;
    case PHASE_SET3:
      dg->tube[TUBE_2] = (sec % 100) / 10;
      dg->tube[TUBE_3] = (min % 10);
      dg->tube[TUBE_4] = (min % 100) / 10;
      dg->tube[TUBE_6] = (hour % 100) / 10;
      break;
    case PHASE_SET5:
      dg->tube[TUBE_2] = (sec % 100) / 10;
      dg->tube[TUBE_3] = (min % 10);
      dg->tube[TUBE_4] = (min % 100) / 10;
      dg->tube[TUBE_5] = (hour % 10);
      dg->tube[TUBE_6] = (hour % 100) / 10;
      break;
    case PHASE_SET1:
      dg->tube[TUBE_1] = (sec % 10);
      dg->tube[TUBE_2] = (sec % 100) / 10;
      dg->tube[TUBE_3] = (min % 10);
      dg->tube[TUBE_4] = (min % 100) / 10;
      dg->tube[TUBE_5] = (hour % 10);
      dg->tube[TUBE_6] = (hour % 100) / 10;
      break;
      break;
    case PHASE_ALL_RAND:
    case PHASE_NONE:
    default:
      break;
    }
    dg_set_tube2disp();
    hmain.display_num = dg->disp;
  }
}

void dg_set_tube2disp(void){
  NixieDiverGenceHandle dg = &hmain.dg;
  dg->disp = 0;
  dg->disp += dg->tube[0];
  dg->disp += dg->tube[1] * 10;
  dg->disp += dg->tube[2] * 100;
  dg->disp += dg->tube[3] * 1000;
  dg->disp += dg->tube[4] * 10000;
  dg->disp += dg->tube[5] * 100000;
}

void nixie_display_off_by_illumination(void){
  if(hmain.adc.illumination > 0){
    nixie_drive_tube_select(DRIVE_ALL);
  }
  else{
    nixie_drive_tube_select(DRIVE_OFF);
  }
}

void nixie_drive_tube_select(DRIVE_SEL_TUBE_t sel){
  NixieDriveTubeHandle tube = &hmain.tube;

  switch(sel){
  case DRIVE_OFF:
  default:
    tube->drive.tube1 = NIXIE_DRIVE_OFF;
    tube->drive.tube2 = NIXIE_DRIVE_OFF;
    tube->drive.tube3 = NIXIE_DRIVE_OFF;
    tube->drive.tube4 = NIXIE_DRIVE_OFF;
    tube->drive.tube5 = NIXIE_DRIVE_OFF;
    tube->drive.tube6 = NIXIE_DRIVE_OFF;
    break;
  case DRIVE_12:
    tube->drive.tube1 = NIXIE_DRIVE_ON;
    tube->drive.tube2 = NIXIE_DRIVE_ON;
    tube->drive.tube3 = NIXIE_DRIVE_OFF;
    tube->drive.tube4 = NIXIE_DRIVE_OFF;
    tube->drive.tube5 = NIXIE_DRIVE_OFF;
    tube->drive.tube6 = NIXIE_DRIVE_OFF;
    break;
  case DRIVE_34:
    tube->drive.tube1 = NIXIE_DRIVE_OFF;
    tube->drive.tube2 = NIXIE_DRIVE_OFF;
    tube->drive.tube3 = NIXIE_DRIVE_ON;
    tube->drive.tube4 = NIXIE_DRIVE_ON;
    tube->drive.tube5 = NIXIE_DRIVE_OFF;
    tube->drive.tube6 = NIXIE_DRIVE_OFF;
    break;
  case DRIVE_56:
    tube->drive.tube1 = NIXIE_DRIVE_OFF;
    tube->drive.tube2 = NIXIE_DRIVE_OFF;
    tube->drive.tube3 = NIXIE_DRIVE_OFF;
    tube->drive.tube4 = NIXIE_DRIVE_OFF;
    tube->drive.tube5 = NIXIE_DRIVE_ON;
    tube->drive.tube6 = NIXIE_DRIVE_ON;
    break;
  case DRIVE_ALL:
    tube->drive.tube1 = NIXIE_DRIVE_ON;
    tube->drive.tube2 = NIXIE_DRIVE_ON;
    tube->drive.tube3 = NIXIE_DRIVE_ON;
    tube->drive.tube4 = NIXIE_DRIVE_ON;
    tube->drive.tube5 = NIXIE_DRIVE_ON;
    tube->drive.tube6 = NIXIE_DRIVE_ON;
    break;
  }
}
