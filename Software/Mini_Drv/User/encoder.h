#ifndef __ENCODER_H__
#define __ENCODER_H__
/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  float last_pulse;
  float rpm;
  float lfp_rpm;
} Speed_t;

/* Private extern variables --------------------------------------------------*/
extern Speed_t speed;
/* Private function declaration ----------------------------------------------*/
float AS5047_Pulse(void);
float encoder_read(void);
void Speed_Cal(Speed_t *speed, float cur_pulse);
#endif
