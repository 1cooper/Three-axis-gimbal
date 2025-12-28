/* Private includes ----------------------------------------------------------*/
#include "fliter.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
IIR_BUTTERWORTH_DEF Rpm_IIR_LPF;
IIR_BUTTERWORTH_DEF Va_IIR_LPF;
IIR_BUTTERWORTH_DEF Vb_IIR_LPF;
IIR_BUTTERWORTH_DEF SmoWe_IIR_LPF;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void IIR_Butterworth_Coefficient_Init(float temp[8], IIR_BUTTERWORTH_DEF* iir_butterworth_temp)
{
  iir_butterworth_temp->b0      = temp[0];
  iir_butterworth_temp->b1      = temp[1];
  iir_butterworth_temp->b2      = temp[2];
  iir_butterworth_temp->a0      = temp[3];
  iir_butterworth_temp->a1      = temp[4];
  iir_butterworth_temp->a2      = temp[5];
  iir_butterworth_temp->gain0   = temp[6];
  iir_butterworth_temp->gain1   = temp[7];
  iir_butterworth_temp->states0 = 0.0f;
  iir_butterworth_temp->states1 = 0.0f;
}

void IIR_Butterworth(float in, float* out, IIR_BUTTERWORTH_DEF* iir_butterworth_temp)
{
  float temp;
  temp = (iir_butterworth_temp->gain0 * in - iir_butterworth_temp->a1 * iir_butterworth_temp->states0) - iir_butterworth_temp->a2 * iir_butterworth_temp->states1;
  *out = ((iir_butterworth_temp->b0 * temp + iir_butterworth_temp->b1 * iir_butterworth_temp->states0) + iir_butterworth_temp->b2 * iir_butterworth_temp->states1) * iir_butterworth_temp->gain1;

  iir_butterworth_temp->states1 = iir_butterworth_temp->states0;
  iir_butterworth_temp->states0 = temp;
}

void IIR_Butterworth_Init()
{
  float Rpm_IIR_Param[8] = {1.0f, 2.0f, 1.0f, 1.0f, -1.77863177782458481424f, 0.80080264666570755f, 0.005542717210280681f, 1.0f};
  IIR_Butterworth_Coefficient_Init(Rpm_IIR_Param, &Rpm_IIR_LPF);
  // 2.5k
  float Va_IIR_Param[8] = {1.0f, 2.0f, 1.0f, 1.0f, -1.543121131237385812f, 0.629604004743374079f, 0.0216207183764970181f, 1.0f};
  IIR_Butterworth_Coefficient_Init(Va_IIR_Param, &Va_IIR_LPF);
  float Vb_IIR_Param[8] = {1.0f, 2.0f, 1.0f, 1.0f, -1.543121131237385812f, 0.629604004743374079f, 0.0216207183764970181f, 1.0f};
  IIR_Butterworth_Coefficient_Init(Vb_IIR_Param, &Vb_IIR_LPF);
  float SmoWe_IIR_Param[8] = {1.0f, 2.0f, 1.0f, 1.0f, -1.109228792618426995f, 0.398152293921439692f, 0.0722308753257531743f, 1.0f};
  IIR_Butterworth_Coefficient_Init(SmoWe_IIR_Param, &SmoWe_IIR_LPF);
}

float LPF_Filter(float alpha, float data, float last_data)
{
  data      = alpha * data + (1 - alpha) * last_data;
  last_data = data;
  return data;
}
