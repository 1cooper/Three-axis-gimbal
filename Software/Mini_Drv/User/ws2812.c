/* Private includes ----------------------------------------------------------*/
#include "ws2812.h"
#include "main.h"
#include "user_lib.h"
/* Private define ------------------------------------------------------------*/
#define WS2812_LowLevel  0xC0  // 0
#define WS2812_HighLevel 0xF0  // 1
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
WS2812 ws2812;
/* Private function prototypes -----------------------------------------------*/

void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t txbuf[24];
  uint8_t res = 0;
  for(int i = 0; i < 8; i++)
  {
    txbuf[7 - i]  = (((g >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
    txbuf[15 - i] = (((r >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
    txbuf[23 - i] = (((b >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
  }
  HAL_SPI_Transmit(&WS2812_SPI_UNIT, &res, 0, 0xFFFF);
  while(WS2812_SPI_UNIT.State != HAL_SPI_STATE_READY);
  HAL_SPI_Transmit(&WS2812_SPI_UNIT, txbuf, 24, 0xFFFF);
  for(int i = 0; i < 100; i++)
  {
    HAL_SPI_Transmit(&WS2812_SPI_UNIT, &res, 1, 0xFFFF);
  }
}

void WS2812_Breathing_Param_Init(WS2812 *ws2812, uint8_t r, uint8_t g, uint8_t b, float breathe_speed, float min_brightness, float max_brightness)
{
  ws2812->base_r         = r;
  ws2812->base_g         = g;
  ws2812->base_b         = b;
  ws2812->breathe_speed  = breathe_speed;
  ws2812->min_brightness = min_brightness;
  ws2812->max_brightness = max_brightness;
}

void WS2812_BreathEffect_Smooth(WS2812 *ws2812)
{
  ws2812->base     = (sinf(ws2812->breathangle) + 1.0f) / 2.0f;
  float brightness = ws2812->min_brightness + ws2812->base * (ws2812->max_brightness - ws2812->min_brightness);

  if(ws2812->base > 0.05f)
  {
    ws2812->ready_to_switch = 1;
  }

  uint8_t r = (uint8_t)(ws2812->base_r * brightness);
  uint8_t g = (uint8_t)(ws2812->base_g * brightness);
  uint8_t b = (uint8_t)(ws2812->base_b * brightness);

  WS2812_Ctrl(r, g, b);

  ws2812->breathangle += ws2812->breathe_speed;
  if(ws2812->breathangle >= 2 * PI) ws2812->breathangle -= 2 * PI;
}
