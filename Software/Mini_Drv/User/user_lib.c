/* Private includes ----------------------------------------------------------*/
#include "user_lib.h"
#include "data.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void sort(uint16_t *avg_pulse, uint16_t val)
{
  float temp;
  for(uint16_t i = 0; i <= val - 1; i++)
  {
    for(uint16_t j = 0; j <= val - 1; j++)
    {
      if(avg_pulse[j] > avg_pulse[j + 1])
      {
        temp             = avg_pulse[j];
        avg_pulse[j]     = avg_pulse[j + 1];
        avg_pulse[j + 1] = temp;
      }
    }
  }
}

void average(uint16_t *avg_pulse1, uint16_t *avg_pulse2, uint16_t val)
{
  for(int i = 0; i <= val - 1; i++)
  {
    avg_pulse1[i] = (uint16_t)(avg_pulse1[i] + avg_pulse2[i]) / 2.0f;
  }
}

float find_closest_avg(uint16_t *avg_pulse, float target, uint16_t pole)
{
  float nearest  = avg_pulse[0];
  float min_diff = fabs(nearest - target);
  float diff     = 0;
  for(int i = 1; i < pole; i++)
  {
    diff = fabs(avg_pulse[i] - target);
    if(diff < min_diff)
    {
      min_diff = diff;
      nearest  = avg_pulse[i];
    }
  }
  return nearest;
}

float find_closest_avg2(uint16_t lut[256], float target, uint16_t pole)
{
  float nearest  = lut[0];
  float min_diff = fabs(nearest - target);
  float diff     = 0;
  for(int i = 1; i < pole; i++)
  {
    diff = fabs(lut[i] - target);
    if(diff < min_diff)
    {
      min_diff = diff;
      nearest  = lut[i];
    }
  }
  return nearest;
}

uint16_t find_closest_value(uint16_t avg_pulse[256], uint16_t a)
{
  if(256 == 0) return 0;  // 安全保护（实际数组固定为256）

  uint32_t min_diff = UINT32_MAX;
  uint16_t closest  = avg_pulse[0];  // 初始化为数组第一个元素

  for(int i = 0; i < 256; i++)
  {
    // 无符号差值计算，避免溢出
    uint32_t diff = (avg_pulse[i] >= a) ? (avg_pulse[i] - a) : (a - avg_pulse[i]);

    // 发现更小差值时更新
    if(diff < min_diff)
    {
      min_diff = diff;
      closest  = avg_pulse[i];
    }
  }
  return closest;
}

void offset_minner(uint16_t lut[256], uint16_t offset)
{
  for(uint16_t i; i < 256; i++)
  {
    float diff;
    diff = lut[i] - offset;
    if(diff < 0)
    {
      lut[i] = 16383 + lut[i] - offset;
    }
    else
    {
      lut[i] = lut[i] - offset;
    }
  }
}
// ws2812使用
int constrain(int x, int a, int b)
{
  if(x < a)
    return a;
  else if(x > b)
    return b;
  else
    return x;
}

uint32_t millis()
{
  return HAL_GetTick();  // HAL库默认提供
}

float Limit_Angle(float ElAngle)
{
  while(ElAngle > _2_PI || ElAngle < 0)
  {
    if(ElAngle > _2_PI)
    {
      ElAngle -= _2_PI;
    }
    else if(ElAngle < 0)
    {
      ElAngle += _2_PI;
    }
  }

  return ElAngle;
}

float Limit(float data, float lim)
{
  float temp = data;
  if(fabs(temp) > lim)
  {
    if(temp > 0)
      temp = lim;
    else
      temp = -lim;
  }
  return temp;
}

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
  /// converts unsigned int to float, given range and number of bits ///
  float span   = x_max - x_min;
  float offset = x_min;
  return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

int float_to_uint(float x, float x_min, float x_max, int bits)
{
  /// Converts a float to an unsigned int, given range and number of bits ///
  float span   = x_max - x_min;
  float offset = x_min;
  return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}

// float限幅
float float_constrain(float Value, float minValue, float maxValue)
{
  if(Value < minValue)
    return minValue;
  else if(Value > maxValue)
    return maxValue;
  else
    return Value;
}

void process_numbers(float num1, float num2, float num3, float *avg1, float *avg2, float *avg3)
{
  static int call_count = 0;
  static float sum1 = 0.0f, sum2 = 0.0f, sum3 = 0.0f;
  // 累加输入值
  sum1 += num1;
  sum2 += num2;
  sum3 += num3;

  call_count++;

  // 当调用达到1000次时，计算并输出平均值
  if(call_count == 1000)
  {
    *avg1 = sum1 / 1000;
    *avg2 = sum2 / 1000;
    *avg3 = sum3 / 1000;

    // 重置计数器用于后续可能的计算
    call_count = 0;
    sum1       = 0.0f;
    sum2       = 0.0f;
    sum3       = 0.0f;
  }
}
