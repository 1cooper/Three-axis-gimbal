#ifndef __TASK_CFG_H__
#define __TASK_CFG_H__
/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private extern variables --------------------------------------------------*/
extern int16_t offset_lut[256];
extern uint16_t encoder_offset;
/* Private function declaration ----------------------------------------------*/
void Task_Init(void);
void Bsp_Init(void);
void Param_Init(void);
void Decives_Init(void);
#endif
