/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Lightning_ADC_Pin GPIO_PIN_1
#define Lightning_ADC_GPIO_Port GPIOA
#define Nixie_A_Pin GPIO_PIN_4
#define Nixie_A_GPIO_Port GPIOA
#define Nixie_B_Pin GPIO_PIN_5
#define Nixie_B_GPIO_Port GPIOA
#define Nixie_C_Pin GPIO_PIN_6
#define Nixie_C_GPIO_Port GPIOA
#define Nixie_D_Pin GPIO_PIN_7
#define Nixie_D_GPIO_Port GPIOA
#define Nixie_6_SEL_Pin GPIO_PIN_0
#define Nixie_6_SEL_GPIO_Port GPIOB
#define Nixie_5_SEL_Pin GPIO_PIN_1
#define Nixie_5_SEL_GPIO_Port GPIOB
#define Nixie_4_SEL_Pin GPIO_PIN_8
#define Nixie_4_SEL_GPIO_Port GPIOA
#define Nixie_3_SEL_Pin GPIO_PIN_9
#define Nixie_3_SEL_GPIO_Port GPIOA
#define Nixie_2_SEL_Pin GPIO_PIN_10
#define Nixie_2_SEL_GPIO_Port GPIOA
#define Nixie_1_SEL_Pin GPIO_PIN_11
#define Nixie_1_SEL_GPIO_Port GPIOA
#define PWM_LED_Pin GPIO_PIN_12
#define PWM_LED_GPIO_Port GPIOA
#define RTC_SCL_Pin GPIO_PIN_15
#define RTC_SCL_GPIO_Port GPIOA
#define SW_Forward_Pin GPIO_PIN_4
#define SW_Forward_GPIO_Port GPIOB
#define SW_Backward_Pin GPIO_PIN_5
#define SW_Backward_GPIO_Port GPIOB
#define SW_Enter_Pin GPIO_PIN_6
#define SW_Enter_GPIO_Port GPIOB
#define RTC_SDA_Pin GPIO_PIN_7
#define RTC_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
