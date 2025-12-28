#ifndef __CANBSP_H__
#define __CANBSP_H__

/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

typedef struct
{
  /*receive data*/
  volatile float receive_current;
  volatile uint16_t receive_current_u16;
  /*pulse*/
  volatile float send_pulse;
  volatile uint16_t send_pulse_u16;
  /*speed*/
  volatile float send_speed;
  volatile uint16_t send_speed_u16;
  /*current*/
  volatile float send_current;
  volatile uint16_t send_current_u16;
} MOTOR_DATA;

/* Private extern variables --------------------------------------------------*/
extern uint8_t can_heartbeat;
extern MOTOR_DATA motor_data;
/* Private function declaration ----------------------------------------------*/
void Send_Motor_Message(uint16_t id);
#endif
