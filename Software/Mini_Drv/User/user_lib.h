#ifndef __USER_LIB_H__
#define __USER_LIB_H__

/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private extern variables --------------------------------------------------*/
/* Private function declaration ----------------------------------------------*/
void sort(uint16_t *avg_pulse, uint16_t val);
void average(uint16_t *avg_pulse1, uint16_t *avg_pulse2, uint16_t val);
float find_closest_avg(uint16_t *avg_pulse, float target, uint16_t pole);
float find_closest_avg2(uint16_t lut[256], float target, uint16_t pole);
uint16_t find_closest_value(uint16_t avg_pulse[256], uint16_t a);
void offset_minner(uint16_t lut[256], uint16_t offset);
int constrain(int x, int a, int b);
uint32_t millis(void);
float Limit_Angle(float ElAngle);
float Limit(float data, float lim);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
int float_to_uint(float x, float x_min, float x_max, int bits);
float float_constrain(float Value, float minValue, float maxValue);
void process_numbers(float num1, float num2, float num3, float *avg1, float *avg2, float *avg3);
#endif
