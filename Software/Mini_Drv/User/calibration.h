#ifndef __CALIBIRATION_H__
#define __CALIBIRATION_H__

/* Private includes ----------------------------------------------------------*/
#include "foc.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/

#define ENCODER_CPR_F          2097152.0f
#define ENCODER_CPR            2097152
/* Private typedef -----------------------------------------------------------*/

typedef enum eCalibStep
{
  CS_NULL = 0,
  CS_ENCODER_START,
  CS_ENCODER_CW_LOOP,
  CS_ENCODER_CCW_LOOP,
  CS_ENCODER_DP,
  CS_ENCODER_SAVE_LUT,
  CS_ENCODER_END
} tCalibStep;

typedef struct
{
  /* data */
  float count_in_cpr;
  float pos_cpr_counts;
  float vel_estimate_counts;
  float pll_kp;
  float pll_ki;
} tEncoderLoop;

/* Private extern variables --------------------------------------------------*/
extern tEncoderLoop Encoder;
/* Private function declaration ----------------------------------------------*/
void Calibration_Loop(FOC *foc);
float Encoder_Comp(tEncoderLoop *Encoder, int16_t *offset_lut, uint32_t encoder_raw, uint32_t offset);
void Encoder_Init(tEncoderLoop *Encoder, float kp, float ki);
#endif
