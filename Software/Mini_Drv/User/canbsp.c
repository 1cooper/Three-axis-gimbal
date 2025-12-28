/* Private includes ----------------------------------------------------------*/
#include "canbsp.h"
#include "fdcan.h"
#include "user_lib.h"
/* Private define ------------------------------------------------------------*/
#define RIGHT_MOTOR 0x201
#define LEFT_MOTOR  0x202
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t can_rx_data[8];
uint8_t can_heartbeat;
MOTOR_DATA motor_data;
/* Private function prototypes -----------------------------------------------*/

void can_filter_init(void)
{
  FDCAN_FilterTypeDef fdcan_filter;

  fdcan_filter.IdType       = FDCAN_STANDARD_ID;  // 标准ID
  fdcan_filter.FilterIndex  = 0;
  fdcan_filter.FilterType   = FDCAN_FILTER_MASK;
  fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  fdcan_filter.FilterID1    = 0x00;
  fdcan_filter.FilterID2    = 0x00;

  HAL_FDCAN_ConfigFilter(&hfdcan1, &fdcan_filter);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    FDCAN_RxHeaderTypeDef fdcan_RxHeader;
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &fdcan_RxHeader, can_rx_data);
    switch(fdcan_RxHeader.Identifier)
    {
      case LEFT_MOTOR:
      {
        can_heartbeat                  = 0;
        motor_data.receive_current_u16 = can_rx_data[0] << 8 | can_rx_data[1];
        motor_data.receive_current     = uint_to_float(motor_data.receive_current_u16, -20.0f, 20.0f, 16);
      }
      break;
    }
  }
}

void Send_Motor_Message(uint16_t id)
{
  FDCAN_TxHeaderTypeDef TxHeader;
  uint8_t can_send_data[8];

  TxHeader.Identifier          = id;
  TxHeader.IdType              = FDCAN_STANDARD_ID;   // 标准ID
  TxHeader.TxFrameType         = FDCAN_DATA_FRAME;    // 数据帧
  TxHeader.DataLength          = FDCAN_DLC_BYTES_2;   // 发送数据长度
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;    // 设置错误状态指示
  TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;       // 不开启可变波特率
  TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;   // 普通CAN格式
  TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;  // 用于发送事件FIFO控制, 不存储
  TxHeader.MessageMarker       = 0x52;

  /*send data*/
  motor_data.send_pulse_u16 = float_to_uint(motor_data.send_pulse, 0.0f, 16383.0f, 16);
  can_send_data[0]          = motor_data.send_pulse_u16 >> 8;
  can_send_data[1]          = motor_data.send_pulse_u16;

  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, can_send_data);
}
