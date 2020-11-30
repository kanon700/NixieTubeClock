/*
 * nixie_ui.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_UI_H_
#define INC_NIXIE_NIXIE_UI_H_

#include "main.h"

#define NIXIE_UI_BTN_FORWARD 0x01
#define NIXIE_UI_BTN_BACKWARD 0x02
#define NIXIE_UI_BTN_ENTER 0x04

typedef struct{
  uint8_t btn_event;
  uint8_t btn_state;
  uint32_t press_time_fw;
  uint32_t press_time_bw;
  uint32_t press_time_en;
}NixieUiHandle_t;
typedef NixieUiHandle_t* NixieUiHandle;

extern void nixie_ui_btn_callback(void);
extern uint8_t nixie_ui_btn_get_event(void);
extern void nixie_ui_btn_clear_event(void);

#endif /* INC_NIXIE_NIXIE_UI_H_ */
