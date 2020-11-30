/*
 * nixie_adc.c
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include "stm32f3xx_hal.h"
#include "nixie_adc.h"

NixieAdcHandle_t adc_handle = {0};

// adc値を0～100に変換して返す
void nixie_get_illumination(uint8_t *data){
  uint16_t max = 3300;
  uint16_t min = 600;
  float out;
  uint16_t light = adc_handle.out.adc_illumination;

  if(light < min){
    light = min;
  }
  else if (light > max){
    light = max;
  }
  light -= min;

  out = light / ((max - min) / 100);
  if(out > 100){
    out = 100;
  }
  *data = (uint8_t)out;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
  adc_handle.out.adc_illumination = adc_handle.work.retdma[0];
//  adc_handle.out.adc_mcu_temperature = adc_handle.work.retdma[1];
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc){
  HAL_ADC_Stop_DMA(&hadc1);
}

HAL_StatusTypeDef nixie_adc_exec(void){
  HAL_StatusTypeDef sts = HAL_OK;
  if(READ_BIT(hadc1.State, HAL_ADC_STATE_READY) != 0){
    sts = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_handle.work.retdma, ADC1_NUM);
  }

  return sts;
}
