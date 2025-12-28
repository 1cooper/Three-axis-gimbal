/* Private includes ----------------------------------------------------------*/
#include "foc.h"
#include "data.h"
#include "encoder.h"
#include "flash.h"
//#include "hfi_q.h"
#include "main.h"
#include "tim.h"
#include "user_lib.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
FOC foc;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

uint8_t Sector_Judge(float u_alpha, float u_beta)
{
  float A = u_beta;
  float B = u_alpha * sqrt3 / 2.0f - u_beta / 2.0f;
  float C = -u_alpha * sqrt3 / 2.0f - u_beta / 2.0f;

  uint8_t N = 0;
  if(A > 0)
  {
    N = N + 1;
  }
  if(B > 0)
  {
    N = N + 2;
  }
  if(C > 0)
  {
    N = N + 4;
  }
  return N;
}

void SVPWM(FOC *foc)
{
  /*常量×基本相*/
  uint8_t N = Sector_Judge(foc->u_alpha, foc->u_beta);
  float A_X = (sqrt3 * T_s) / Udc * foc->u_beta;
  float B_Y = (sqrt3 * T_s) / Udc * ((sqrt3 / 2.0f) * foc->u_alpha - foc->u_beta / 2.0f);
  float C_Z = (sqrt3 * T_s) / Udc * (-(sqrt3 / 2.0f) * foc->u_alpha - foc->u_beta / 2.0f);
  switch(N)
  {
    case 3:
    {
      foc->t_a = B_Y;
      foc->t_b = A_X;
      break;
    } /*第一扇区*/
    case 1:
    {
      foc->t_a = -C_Z;
      foc->t_b = -B_Y;
      break;
    } /*第二扇区*/
    case 5:
    {
      foc->t_a = A_X;
      foc->t_b = C_Z;
      break;
    } /*第三扇区*/
    case 4:
    {
      foc->t_a = -B_Y;
      foc->t_b = -A_X;
      break;
    } /*第四扇区*/
    case 6:
    {
      foc->t_a = C_Z;
      foc->t_b = B_Y;
      break;
    } /*第五扇区*/
    case 2:
    {
      foc->t_a = -A_X;
      foc->t_b = -C_Z;
      break;
    } /*第六扇区*/
  }
  foc->t_c = (T_s - foc->t_a - foc->t_b) / 2.0f;
  foc->t_a = foc->t_a / T_s * ARR;
  foc->t_b = foc->t_b / T_s * ARR;
  foc->t_c = foc->t_c / T_s * ARR;

  switch(N)
  {
    case 3: /*第一扇区*/
    {
      foc->CCR1 = foc->t_a + foc->t_b + foc->t_c;
      foc->CCR2 = foc->t_b + foc->t_c;
      foc->CCR3 = foc->t_c;
      break;
    }
    case 1: /*第二扇区*/
    {
      foc->CCR1 = foc->t_a + foc->t_c;
      foc->CCR2 = foc->t_a + foc->t_b + foc->t_c;
      foc->CCR3 = foc->t_c;
      break;
    }
    case 5: /*第三扇区*/
    {
      foc->CCR1 = foc->t_c;
      foc->CCR2 = foc->t_a + foc->t_b + foc->t_c;
      foc->CCR3 = foc->t_b + foc->t_c;
      break;
    }
    case 4: /*第四扇区*/
    {
      foc->CCR1 = foc->t_c;
      foc->CCR2 = foc->t_a + foc->t_c;
      foc->CCR3 = foc->t_a + foc->t_b + foc->t_c;
      break;
    }
    case 6: /*第五扇区*/
    {
      foc->CCR1 = foc->t_b + foc->t_c;
      foc->CCR2 = foc->t_c;
      foc->CCR3 = foc->t_a + foc->t_b + foc->t_c;
      break;
    }
    case 2: /*第六扇区*/
    {
      foc->CCR1 = foc->t_a + foc->t_b + foc->t_c;
      foc->CCR2 = foc->t_c;
      foc->CCR3 = foc->t_a + foc->t_c;
      break;
    }
  }
  TIM1->CCR1 = foc->CCR1;
  TIM1->CCR2 = foc->CCR2;
  TIM1->CCR3 = foc->CCR3;
}

void SinCosCalculate(FOC *foc)
{
  foc->cos_val = arm_cos_f32(foc->theta);
  foc->sin_val = arm_sin_f32(foc->theta);
}

void IparkTrans(FOC *foc)
{
  foc->u_alpha = foc->u_d * foc->cos_val - foc->u_q * foc->sin_val;
  foc->u_beta  = foc->u_d * foc->sin_val + foc->u_q * foc->cos_val;
}

void ClarkeTrans(FOC *foc)
{
  // 双采的时候使用
  foc->i_alpha = foc->i_a;
  foc->i_beta  = third_sqrt3 * foc->i_a + DoubleTripSqrt3 * foc->i_b;
  // 三采的时候使用
  // 在三采样时，判断ccr值，最大的不采，采小的两项，得出两项去计算第三项。
}

void ParkTrans(FOC *foc)
{
  foc->i_d = foc->i_alpha * foc->cos_val + foc->i_beta * foc->sin_val;
  foc->i_q = foc->i_beta * foc->cos_val - foc->i_alpha * foc->sin_val;
}

void FocParamInit(FOC *foc, float u_q, float u_d)
{
  foc->u_q = u_q;
  foc->u_d = u_d;
}

void OpenLoop(FOC *foc, float step_angle)
{
  foc->ol_theta += step_angle;
  foc->theta     = foc->ol_theta;
  SinCosCalculate(foc);
  IparkTrans(foc);
  SVPWM(foc);
}

void Motor_Align(FOC *foc)
{
  FocParamInit(foc, 0.0f, 0.5f);
  foc->theta = 0.0f;
  IparkTrans(foc);
  SVPWM(foc);
}

void Start_Motor(void)
{
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void Stop_Motor(void)
{
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;

  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

  // __HAL_TIM_DISABLE(&htim1);
}
