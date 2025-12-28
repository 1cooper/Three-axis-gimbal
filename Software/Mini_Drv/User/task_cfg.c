/* Private includes ----------------------------------------------------------*/
#include "task_cfg.h"
#include "adc.h"
#include "calibration.h"
#include "encoder.h"
#include "fdcan.h"
#include "flash.h"
#include "fliter.h"
//#include "flux_observer.h"
#include "foc.h"
//#include "hfi_q.h"
#include "main.h"
#include "pid.h"
//#include "smo.h"
#include "tim.h"
#include "ws2812.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*for eangle*/
uint8_t flash_data[520];
int16_t offset_lut[256];
uint16_t encoder_offset;
/* Private function prototypes -----------------------------------------------*/
void Task_Init(void)
{
  /*关闭中断,防止在初始化过程中发生中断*/
  __disable_irq();
  Bsp_Init();
  Param_Init();
  Decives_Init();
  /*初始化完成，开启中断*/
  __enable_irq();
}

void Bsp_Init(void)
{
  /*ADC*/
  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADC_Start_IT(&hadc1);
  /*TIM*/
  HAL_TIM_Base_Start_IT(&htim2);
  /*FOC*/
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  /*CAN*/
//  HAL_FDCAN_Start(&hfdcan1);
//  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}

void Param_Init(void)
{
  /*FOC Param Init*/
  FocParamInit(&foc, 0.5f, 0.0f);
  /*PID Param Init*/
  Curr_PI_Param_VoltCircLInit(&Iqd_PI, 0.0f, 0.0f, 0.0f, 0.0f);
  PID_Init(&Speed_PID, 0.0001f, 0.01f, 0.001f, 1000.0f, 10.0f);
  PID_Init(&Pos_PID, 0.0001f, 0.001f, 0.1f, 1.0f, 200.0f);
  /*RGB Init*/
  WS2812_Breathing_Param_Init(&ws2812, 50, 5, 20, 0.003f, 0.05f, 0.9f);
  /*Eangle Init*/
  ReadFlashData(0x00, flash_data, 520);
  Read_lut_FromData(flash_data, offset_lut, 256);
  Read_offset_FromData(flash_data, &encoder_offset, 256);
  /*HFI Init*/
//  HFI_Init(&hfi);
//  /*FLUX Init*/
//  Flux_Init(&flux);
//  Smo_Init(&smo);
//  /*IIR Init*/
//  IIR_Butterworth_Init();
}

void Decives_Init(void)
{
}
