/*
 * nixie_ui.c
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include "main.h"
#include "nixie_ui.h"
#include "gpio.h"

NixieUiHandle_t hui = {0};

uint8_t nixie_ui_btn_get_event(){
  return hui.btn_event;
}

void nixie_ui_btn_clear_event(){
  hui.btn_event = 0;
  HAL_Delay(20);
}

void nixie_ui_btn_callback(){
  if(HAL_GPIO_ReadPin(SW_Forward_GPIO_Port , SW_Forward_Pin) == GPIO_PIN_SET){
    if((hui.btn_state & NIXIE_UI_BTN_FORWARD) == 0){
      hui.press_time_fw = HAL_GetTick();
    }
    hui.btn_state |= NIXIE_UI_BTN_FORWARD;
  }
  else{
    if((hui.btn_state & NIXIE_UI_BTN_FORWARD) != 0){
      if((HAL_GetTick() - hui.press_time_fw ) > 5){
        hui.btn_event |= NIXIE_UI_BTN_FORWARD;
      }
      hui.btn_state &= ~NIXIE_UI_BTN_FORWARD;
      hui.press_time_fw = HAL_GetTick();
    }
  }


  if(HAL_GPIO_ReadPin(SW_Backward_GPIO_Port , SW_Backward_Pin) == GPIO_PIN_SET){
    if((hui.btn_state & NIXIE_UI_BTN_BACKWARD) == 0){
      hui.press_time_bw = HAL_GetTick();
    }
    hui.btn_state |= NIXIE_UI_BTN_BACKWARD;
  }
  else{
    if((hui.btn_state & NIXIE_UI_BTN_BACKWARD) != 0){
      if((HAL_GetTick() - hui.press_time_bw ) > 5){
        hui.btn_event |= NIXIE_UI_BTN_BACKWARD;
      }
      hui.btn_state &= ~NIXIE_UI_BTN_BACKWARD;
      hui.press_time_bw = HAL_GetTick();
    }
  }

  if(HAL_GPIO_ReadPin(SW_Enter_GPIO_Port , SW_Enter_Pin) == GPIO_PIN_SET){
    if((hui.btn_state & NIXIE_UI_BTN_ENTER) == 0){
      hui.press_time_en = HAL_GetTick();
    }
    hui.btn_state |= NIXIE_UI_BTN_ENTER;
  }
  else{
    if((hui.btn_state & NIXIE_UI_BTN_ENTER) != 0){
      if((HAL_GetTick() - hui.press_time_en ) > 5){
        hui.btn_event |= NIXIE_UI_BTN_ENTER;
      }
      hui.btn_state &= ~NIXIE_UI_BTN_ENTER;
      hui.press_time_en = HAL_GetTick();
    }
  }
}
