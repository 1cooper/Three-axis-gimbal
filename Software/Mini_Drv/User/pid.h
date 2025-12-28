#ifndef __PID_H__
#define __PID_H__
/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

typedef struct
{
  float target;
  float value;
  float error;
  float prev_error;
  float errSum;
  float errSumMax;
  float kp;
  float ki;
  float kd;
  float result;
  float resultMax;
} PID_Type;

typedef struct
{
  float iq_target;
  float iq_actual;
  float iq_err;
  float iq_k_p;
  float iq_k_i;
  float iq_sumerr;
  float iq_output;
  float iq_mod;

  float id_target;
  float id_actual;
  float id_err;
  float id_k_p;
  float id_k_i;
  float id_sumerr;
  float id_output;
  float id_mod;

  float v_to_mod;
  float mod_to_v;
  float modSf;
} PI_Type_Volt;

/* Private extern variables --------------------------------------------------*/
extern PI_Type_Volt Iqd_PI;
extern PID_Type Speed_PID;
extern PID_Type Pos_PID;
/* Private function declaration ----------------------------------------------*/
void PID_Cal(PID_Type *PID_Control, float target, float value);
void Curr_PI_Cal_VoltCircLimit(PI_Type_Volt *pi, float iq_target, float id_target, float iq_actual, float id_actual);
void Curr_PI_Param_VoltCircLInit(PI_Type_Volt *pi, float iq_P, float iq_I, float id_P, float id_I);
void PID_Init(PID_Type *PID_Control, float kp, float ki, float kd, float errSumMax, float resultMax);
#endif
