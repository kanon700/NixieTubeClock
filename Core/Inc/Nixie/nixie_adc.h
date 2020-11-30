/*
 * nixie_adc.h
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include "adc.h"

#define ADC1_NUM 1

typedef struct {
  struct {
    uint16_t retdma[ADC1_NUM];
  } work;
  struct {
    float adc_illumination;
    float adc_mcu_temperature;
  } out;
} NixieAdcHandle_t;
typedef NixieAdcHandle_t *NixieAdcHandle;

extern HAL_StatusTypeDef nixie_adc_exec(void);
extern void nixie_get_illumination(uint8_t *data);
