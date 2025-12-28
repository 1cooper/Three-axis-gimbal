#ifndef __FOC_H__
#define __FOC_H__
/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t CCR1;
  uint32_t CCR2;
  uint32_t CCR3;

  float pulse;
  float u_alpha;
  float u_beta;
  float t_a;
  float t_b;
  float t_c;
  float theta;
  float cos_val;
  float sin_val;
  float u_d;
  float u_q;
  float i_alpha;
  float i_beta;
  float i_a;
  float i_b;
  float i_c;
  float i_d;
  float i_q;
  float ol_theta;
  float u_a;
  float u_b;
  float u_c;
  float u_a_offset;
  float u_b_offset;
  float u_c_offset;

  float etheta;

  uint16_t u_offset[3];
} FOC;
/* Private extern variables --------------------------------------------------*/
extern FOC foc;
/* Private function declaration ----------------------------------------------*/
void SVPWM(FOC *foc);
void FocParamInit(FOC *foc, float u_q, float u_d);
void SinCosCalculate(FOC *foc);
void IparkTrans(FOC *foc);
void ClarkeTrans(FOC *foc);
void ParkTrans(FOC *foc);
void OpenLoop(FOC *foc, float step_angle);
void Motor_Align(FOC *foc);
void Start_Motor(void);
void Stop_Motor(void);
#endif
