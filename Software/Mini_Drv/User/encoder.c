/* Private includes ----------------------------------------------------------*/
#include "encoder.h"
#include "fliter.h"
#include "spi.h"
#include "calibration.h"
/* Private define ------------------------------------------------------------*/
#define AS5047 0
#define MA730  1
#define ContinuousRead 0xA000
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Speed_t speed;
/* Private function prototypes -----------------------------------------------*/

uint16_t Spi1_Readwriteword(uint16_t TxData)
{
  uint16_t rxdata;
  if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&TxData, (uint8_t*)&rxdata, 1, 1000) != HAL_OK)
  {
    rxdata = 0;
  }
  return rxdata;
}

float AS5047_Pulse(void)
{
  uint16_t data;

  HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_RESET);
  data = Spi1_Readwriteword(0xffff);
  HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);

  data = data & 0x3fff;

  return (float)data;
}

uint16_t MA730_Pulse(void)
{
  uint16_t data;

  HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_RESET);
  data = Spi1_Readwriteword(0x0000);
  HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);

  return data >> 2;
}

 uint32_t Angle;
 uint16_t TxData;
 uint16_t RxData[3];
float MT6835_Pulse(void)
{
//  uint32_t Angle;
//  uint16_t TxData;
//  uint16_t RxData[3];
//  uint16_t ContinuousRead = 0xA000;
  HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_RESET);

  TxData = ContinuousRead | 0x0003;
  HAL_SPI_TransmitReceive(&hspi1, (unsigned char*)&TxData, (unsigned char*)&RxData, 3, 100);

  HAL_GPIO_WritePin(ENCODER_CS_GPIO_Port, ENCODER_CS_Pin, GPIO_PIN_SET);

  Angle = (RxData[1] << 5) | (RxData[2] >> 11);
  return (float)Angle;
}

float encoder_read(void)
{
  float read;
#if AS5047
  read = AS5047_Pulse();
#endif
#if MA730
  // read = MA730_Pulse();
  read = MT6835_Pulse();
#endif
  return read;
}

void Speed_Cal(Speed_t* speed, float cur_pulse)
{
  int delta_pulse;
  delta_pulse = cur_pulse - speed->last_pulse;

  if(delta_pulse < -ENCODER_CPR_F/2.0f)
  {
    delta_pulse = delta_pulse + ENCODER_CPR_F;
  }
  else if(delta_pulse > ENCODER_CPR_F/2.0f)
  {
    delta_pulse = delta_pulse - ENCODER_CPR_F;
  }
  speed->last_pulse = cur_pulse;

  speed->rpm = (float)delta_pulse / (ENCODER_CPR_F * 0.001f) * 60.0f;

  IIR_Butterworth(speed->rpm, &speed->lfp_rpm, &Rpm_IIR_LPF);
}
