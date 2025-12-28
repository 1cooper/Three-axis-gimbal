/* Private includes ----------------------------------------------------------*/
#include "pid.h"
#include "data.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PI_Type_Volt Iqd_PI;
PID_Type Speed_PID;
PID_Type Pos_PID;
float PI_modSf_gain = 0.5f;
/* Private function prototypes -----------------------------------------------*/

void PID_Cal(PID_Type* PID_Control, float target, float value)
{
  PID_Control->target = target;
  PID_Control->value  = value;
  PID_Control->error  = target - value;

  PID_Control->errSum += PID_Control->error;

  if(PID_Control->errSum > PID_Control->errSumMax)
    PID_Control->errSum = PID_Control->errSumMax;
  else if(PID_Control->errSum < -PID_Control->errSumMax)
    PID_Control->errSum = -PID_Control->errSumMax;

  float p_out = PID_Control->kp * PID_Control->error;
  float i_out = PID_Control->ki * PID_Control->errSum;
  float d_out = PID_Control->kd * (PID_Control->error - PID_Control->prev_error);

  PID_Control->result = p_out + i_out + d_out;

  if(PID_Control->result > PID_Control->resultMax)
    PID_Control->result = PID_Control->resultMax;
  else if(PID_Control->result < -PID_Control->resultMax)
    PID_Control->result = -PID_Control->resultMax;

  PID_Control->prev_error = PID_Control->error;
}

void PID_Init(PID_Type* PID_Control, float kp, float ki, float kd, float errSumMax, float resultMax)
{
  PID_Control->kp = kp;
  PID_Control->ki = ki;
  PID_Control->kd = kd;

  PID_Control->target     = 0.0f;
  PID_Control->value      = 0.0f;
  PID_Control->error      = 0.0f;
  PID_Control->prev_error = 0.0f;
  PID_Control->errSum     = 0.0f;
  PID_Control->errSumMax  = errSumMax;
  PID_Control->result     = 0.0f;
  PID_Control->resultMax  = resultMax;
}

void Curr_PI_Cal_VoltCircLimit(PI_Type_Volt* pi, float iq_target, float id_target, float iq_actual, float id_actual)
{
  pi->iq_target = iq_target;
  pi->iq_actual = iq_actual;
  pi->iq_err    = pi->iq_target - pi->iq_actual;
  pi->iq_output = pi->iq_k_p * pi->iq_err + pi->iq_k_i * pi->iq_sumerr * T_s;

  pi->id_target = id_target;
  pi->id_actual = id_actual;
  pi->id_err    = pi->id_target - pi->id_actual;
  pi->id_output = pi->id_k_p * pi->id_err + pi->id_k_i * pi->id_sumerr * T_s;

  pi->id_mod = pi->v_to_mod * pi->id_output;
  pi->iq_mod = pi->v_to_mod * pi->iq_output;

  pi->modSf = PI_modSf_gain * sqrt3 / 2.0f / sqrtf(pi->iq_mod * pi->iq_mod + pi->id_mod * pi->id_mod);
  if(pi->modSf < 1.0f)
  {
    pi->id_mod    *= pi->modSf;
    pi->iq_mod    *= pi->modSf;
    pi->id_sumerr *= 0.99f;
    pi->iq_sumerr *= 0.99f;
  }
  else
  {
    pi->id_sumerr += pi->id_err;
    pi->iq_sumerr += pi->iq_err;
  }
}

void Curr_PI_Param_VoltCircLInit(PI_Type_Volt* pi, float iq_P, float iq_I, float id_P, float id_I)
{
  pi->mod_to_v = (2.0f / 3.0f) * Udc;
  pi->v_to_mod = 1.0f / pi->mod_to_v;

  pi->iq_k_p = (MOTOR_L / T_s / 10.0f);
  pi->iq_k_i = (MOTOR_R / T_s / 10.0f);
  pi->id_k_p = (MOTOR_L / T_s / 10.0f);
  pi->id_k_i = (MOTOR_R / T_s / 10.0f);

  // pi->iq_k_p = iq_P;
  // pi->iq_k_i = iq_I;
  // pi->id_k_p = id_P;
  // pi->id_k_i = id_I;
}
