/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
#include "calibration.h"
#include "data.h"
#include "encoder.h"
#include "flash.h"
#include "foc.h"
#include "heap.h"
#include "user_lib.h"
/* Private define ------------------------------------------------------------*/
#define MAX_MOTOR_POLE_PAIRS   30U
#define SAMPLES_PER_PPAIR      256U
#define CURRENT_MEASURE_PERIOD (float)(1.0f / (float)20000)
#define ENCODER_CPR_DIV        (ENCODER_CPR >> 1)
#define OFFSET_LUT_NUM         256U
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// static tCalibStep mCalibStep = CS_NULL;
// static int16_t *p_error_arr = NULL;
uint8_t malloc_err;
tEncoderLoop Encoder;
/* Private function prototypes -----------------------------------------------*/

void Calibration_Loop(FOC* foc)
{
  static const uint16_t ADC_FREQ     = 20000;  // ADC频率
  static const float calib_phase_vel = PI;     // 校准转速
  static uint32_t loop_count;
  static int16_t sample_count;
  static float phase_set;  // 电角度设置
  static int moving_avg;
  static float next_sample_time;
  static int16_t offset_lut[OFFSET_LUT_NUM];
  static uint16_t encoder_offset;
  static uint8_t write_flash_data[520];
  static tCalibStep mCalibStep = CS_NULL;
  static int16_t* p_error_arr  = NULL;
  /*malloc*/
  if(p_error_arr == NULL)
  {
    p_error_arr = HEAP_malloc(MAX_MOTOR_POLE_PAIRS * SAMPLES_PER_PPAIR * sizeof(int16_t));
    if(p_error_arr == NULL)
    {
      malloc_err = 1;
    }
  }

  float time      = (float)loop_count * CURRENT_MEASURE_PERIOD;
  int encoder_raw = (int)encoder_read();

  switch(mCalibStep)
  {
      /*对齐1s*/
    case CS_NULL:
      if(loop_count < ADC_FREQ)
      {
        foc->u_q   = 0.0f;
        foc->u_d   = 1.5f;
        foc->theta = 0.0f;
        SinCosCalculate(foc);
        IparkTrans(foc);
        SVPWM(foc);
      }
      else
      {
        phase_set        = 0;
        loop_count       = 0;
        sample_count     = 0;
        next_sample_time = 0;
        mCalibStep       = CS_ENCODER_CW_LOOP;
      }
      break;
      /*正转*/
    case CS_ENCODER_CW_LOOP:
      if(sample_count < (MOTOR_POLE * SAMPLES_PER_PPAIR))
      {
        if(time > next_sample_time)
        {
          next_sample_time += _2_PI / ((float)SAMPLES_PER_PPAIR * calib_phase_vel);

          int count_ref              = (phase_set * ENCODER_CPR_F) / (_2_PI * (float)MOTOR_POLE);
          int error                  = encoder_raw - count_ref;
          error                     += ENCODER_CPR * (error < 0);
          p_error_arr[sample_count]  = error;
          sample_count++;
        }

        phase_set += calib_phase_vel * CURRENT_MEASURE_PERIOD;
      }
      else
      {
        phase_set  -= calib_phase_vel * CURRENT_MEASURE_PERIOD;
        loop_count  = 0;
        sample_count--;
        next_sample_time = 0;
        mCalibStep       = CS_ENCODER_CCW_LOOP;
      }
      foc->theta = phase_set;
      SinCosCalculate(foc);
      IparkTrans(foc);
      SVPWM(foc);
      break;
      /*反转*/
    case CS_ENCODER_CCW_LOOP:
      if(sample_count >= 0)
      {
        if(time > next_sample_time)
        {
          next_sample_time += _2_PI / ((float)SAMPLES_PER_PPAIR * calib_phase_vel);

          int count_ref  = (phase_set * ENCODER_CPR_F) / (_2_PI * (float)MOTOR_POLE);
          int error      = encoder_raw - count_ref;
          error         += ENCODER_CPR * (error < 0);

          p_error_arr[sample_count] = (p_error_arr[sample_count] + error) / 2;
          sample_count--;
        }

        phase_set -= calib_phase_vel * CURRENT_MEASURE_PERIOD;
      }
      else
      {
        Stop_Motor();
        mCalibStep = CS_ENCODER_DP;
      }
      foc->theta = phase_set;
      SinCosCalculate(foc);
      IparkTrans(foc);
      SVPWM(foc);
      break;
      /*数据处理*/
    case CS_ENCODER_DP:
    {
      for(int i = 0; i < (MOTOR_POLE * SAMPLES_PER_PPAIR); i++)
      {
        moving_avg += p_error_arr[i];
      }
      encoder_offset = moving_avg / (MOTOR_POLE * SAMPLES_PER_PPAIR);

      int window     = SAMPLES_PER_PPAIR;
      int lut_offset = p_error_arr[0] * OFFSET_LUT_NUM / ENCODER_CPR;  // 将编码器的初始值等比例缩放到0~OFFSET_LUT_NUM内
      for(int i = 0; i < OFFSET_LUT_NUM; i++)
      {
        moving_avg = 0;
        for(int j = (-window) / 2; j < (window) / 2; j++)
        {
          int index = i * MOTOR_POLE * SAMPLES_PER_PPAIR / OFFSET_LUT_NUM + j;
          if(index < 0)
          {
            index += (SAMPLES_PER_PPAIR * MOTOR_POLE);
          }
          else if(index > (SAMPLES_PER_PPAIR * MOTOR_POLE - 1))
          {
            index -= (SAMPLES_PER_PPAIR * MOTOR_POLE);
          }
          moving_avg += p_error_arr[index];
        }
        moving_avg    = moving_avg / window;
        int lut_index = lut_offset + i;
        if(lut_index > (OFFSET_LUT_NUM - 1))
        {
          lut_index -= OFFSET_LUT_NUM;
        }
        offset_lut[lut_index] = moving_avg - encoder_offset;
      }
      mCalibStep = CS_ENCODER_SAVE_LUT;
    }
    break;
    case CS_ENCODER_SAVE_LUT:
    {
      Write_lut_ToData(offset_lut, write_flash_data, 256);  // num= 16位数据个数
      Write_Offset_ToData(encoder_offset, write_flash_data, 256);
      WriteFlashData(0x00, write_flash_data, 514);  // num= 8位数据个数
      /*释放*/
      HEAP_free(p_error_arr);
      p_error_arr = NULL;
      mCalibStep  = CS_ENCODER_END;
    }
    break;
    case CS_ENCODER_END:
    {
    }
    break;
  }

  loop_count++;
}

float Encoder_Comp(tEncoderLoop* Encoder, int16_t* offset_lut, uint32_t encoder_raw, uint32_t offset)
{
//  int off_1      = offset_lut[(encoder_raw) >> 6];
//  int off_2      = offset_lut[((encoder_raw >> 6) + 1) % 256];
//  int off_interp = off_1 + ((off_2 - off_1) * (encoder_raw - ((encoder_raw >> 6) << 6)) >> 6);
  /*Compensate Angle in Mechanical*/
//  float corrected_count = encoder_raw - off_interp;
  float corrected_count = encoder_raw;
  if(corrected_count >= offset)
  {
    corrected_count = (corrected_count - (float)offset) / ENCODER_CPR_F * _2_PI;
  }
  else if(corrected_count < offset)
  {
    corrected_count = (ENCODER_CPR_F + corrected_count - (float)offset) / ENCODER_CPR_F * _2_PI;
  }
  float e_theta = fmodf(corrected_count * MOTOR_POLE, _2_PI);
  return e_theta;
}

void Encoder_Init(tEncoderLoop* Encoder, float kp, float ki)
{
  Encoder->pll_kp = kp;
  Encoder->pll_ki = ki;
}
