#ifndef __FLITER_H__
#define __FLITER_H__

/* Private includes ----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  float states0;
  float states1;
  float b0;
  float b1;
  float b2;
  float a0;
  float a1;
  float a2;
  float gain0;
  float gain1;
} IIR_BUTTERWORTH_DEF;
/* Private extern variables --------------------------------------------------*/
extern IIR_BUTTERWORTH_DEF Rpm_IIR_LPF;
extern IIR_BUTTERWORTH_DEF Va_IIR_LPF;
extern IIR_BUTTERWORTH_DEF Vb_IIR_LPF;
extern IIR_BUTTERWORTH_DEF SmoWe_IIR_LPF;
/* Private function declaration ----------------------------------------------*/
void IIR_Butterworth_Coefficient_Init(float temp[8], IIR_BUTTERWORTH_DEF* iir_butterworth_temp);
void IIR_Butterworth(float in, float* out, IIR_BUTTERWORTH_DEF* iir_butterworth_temp);
void IIR_Butterworth_Init(void);
float LPF_Filter(float alpha, float data, float last_data);
#endif
