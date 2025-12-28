#ifndef __WS2812_H__
#define __WS2812_H__
/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
#define WS2812_SPI_UNIT hspi3
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  float base;
  float breathangle;
  float base_r;
  float base_g;
  float base_b;
  float min_brightness;  // 最小亮度
  float max_brightness;  // 最大亮度
  float breathe_speed;   // 呼吸速度（值越大越快）
  float ready_to_switch;
} WS2812;

/* Private extern variables --------------------------------------------------*/
extern SPI_HandleTypeDef WS2812_SPI_UNIT;
extern WS2812 ws2812;
/* Private function declaration ----------------------------------------------*/
void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);
void WS2812_Breathing(WS2812 *ws2812, uint8_t R, uint8_t G, uint8_t B);
void WS2812_Breathing_Param_Init(WS2812 *ws2812, uint8_t r, uint8_t g, uint8_t b, float breathe_speed, float min_brightness, float max_brightness);
void WS2812_BreathEffect_Smooth(WS2812 *ws2812);
#endif
