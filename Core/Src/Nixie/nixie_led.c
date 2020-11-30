/*
 * nixie_led.c
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include "nixie_led.h"
#include "tim.h"

void nixie_led_init(void) {
  __HAL_TIM_SET_COMPARE(&htim16,  TIM_CHANNEL_1, 0);
  HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
}

void nixie_update_led(uint8_t led){
  if(led > 100){
    led = 100;
  }
  __HAL_TIM_SET_COMPARE(&htim16,  TIM_CHANNEL_1, led);
}
