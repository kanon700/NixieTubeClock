/*
 * nixie_led.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#ifndef INC_NIXIE_NIXIE_LED_H_
#define INC_NIXIE_NIXIE_LED_H_

#include "stm32f3xx_hal.h"

extern void nixie_led_init(void);
extern void nixie_update_led(uint8_t led);

#endif /* INC_NIXIE_NIXIE_LED_H_ */
