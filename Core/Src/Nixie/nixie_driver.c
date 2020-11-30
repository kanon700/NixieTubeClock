/*
 * nixie_driver.c
 *
 *  Created on: Oct 31, 2020
 *      Author: kanon700
 */

#include "stm32f3xx_hal.h"
#include "gpio.h"
#include "nixie_driver.h"

NixieDriverHandle_t hdriver = {0};

static void select_drive_nixietube(uint8_t sel);
static void all_off_nixietube(void);
static void control_timing(void);
static void drive_nixie(void);
static void calc_set_num(uint32_t num);
static void set_num_on_nixietube(uint8_t num);

void nixie_driver_task(uint32_t num){
  calc_set_num(num);
  control_timing();
  drive_nixie();
}

void drive_nixie(void){
  if(hdriver.phase == PHASE_ON){
    set_num_on_nixietube(hdriver.tubeall[hdriver.sel_tube]);
    select_drive_nixietube(hdriver.sel_tube);
  }
  else{
    all_off_nixietube();
  }
}

void control_timing(void){
  if(hdriver.phase == PHASE_ON){
    if(HAL_GetTick() - hdriver.last_time > NIXIE_ON_TIME){
      hdriver.sel_tube++;
      if(hdriver.sel_tube > TUBE_ALL){
        hdriver.sel_tube = TUBE_1;
      }
      hdriver.phase = PHASE_OFF;
      hdriver.last_time = HAL_GetTick();
    }
  }
  else{
    hdriver.last_time = HAL_GetTick();
    hdriver.phase = PHASE_ON;
  }
}

/*
 * ダイナミック点灯は常に裏で動いていて、上位で各管の点ける/点けないを制御する仕組みが必要
 * 各巻ごとにフラグを持って、それに応じて点ける点けないでいけるかなぁ。
 */

void select_drive_nixietube(TUBU_NUM_t sel){
  switch(sel){
  case TUBE_1:
    HAL_GPIO_WritePin(Nixie_1_SEL_GPIO_Port, Nixie_1_SEL_Pin, GPIO_PIN_SET);
    break;
  case TUBE_2:
    HAL_GPIO_WritePin(Nixie_2_SEL_GPIO_Port, Nixie_2_SEL_Pin, GPIO_PIN_SET);
    break;
  case TUBE_3:
    HAL_GPIO_WritePin(Nixie_3_SEL_GPIO_Port, Nixie_3_SEL_Pin, GPIO_PIN_SET);
    break;
  case TUBE_4:
    HAL_GPIO_WritePin(Nixie_4_SEL_GPIO_Port, Nixie_4_SEL_Pin, GPIO_PIN_SET);
    break;
  case TUBE_5:
    HAL_GPIO_WritePin(Nixie_5_SEL_GPIO_Port, Nixie_5_SEL_Pin, GPIO_PIN_SET);
    break;
  case TUBE_6:
    HAL_GPIO_WritePin(Nixie_6_SEL_GPIO_Port, Nixie_6_SEL_Pin, GPIO_PIN_SET);
    break;
  default:
    break;
  }
}

void all_off_nixietube(void){
  HAL_GPIO_WritePin(Nixie_1_SEL_GPIO_Port, Nixie_1_SEL_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Nixie_2_SEL_GPIO_Port, Nixie_2_SEL_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Nixie_3_SEL_GPIO_Port, Nixie_3_SEL_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Nixie_4_SEL_GPIO_Port, Nixie_4_SEL_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Nixie_5_SEL_GPIO_Port, Nixie_5_SEL_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Nixie_6_SEL_GPIO_Port, Nixie_6_SEL_Pin, GPIO_PIN_RESET);
}

void calc_set_num(uint32_t num){
  hdriver.tube.no1 = num % 10;
  hdriver.tube.no2 = (num % 100)     / 10;
  hdriver.tube.no3 = (num % 1000)    / 100;
  hdriver.tube.no4 = (num % 10000)   / 1000;
  hdriver.tube.no5 = (num % 100000)  / 10000;
  hdriver.tube.no6 = (num % 1000000) / 100000;
}

// nixie管トップビューとボトムビュー間違えたのでそれに合わせてON箇所をスワップしてる。
void set_num_on_nixietube(uint8_t num){
  switch(num){
  case 0:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 1:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 2:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_SET);
    break;
  case 3:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_SET);
    break;
  case 4:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 5:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 6:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 7:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 8:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  case 9:
    HAL_GPIO_WritePin(Nixie_A_GPIO_Port, Nixie_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_B_GPIO_Port, Nixie_B_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Nixie_C_GPIO_Port, Nixie_C_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Nixie_D_GPIO_Port, Nixie_D_Pin, GPIO_PIN_RESET);
    break;
  default:
    break;
  }
}

