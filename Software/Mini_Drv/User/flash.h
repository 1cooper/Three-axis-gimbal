#ifndef __FLASH_H__
#define __FLASH_H__
/* Private includes ----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
#define STARTADDR 0x0801E000
/* Private typedef -----------------------------------------------------------*/
/* Private extern variables --------------------------------------------------*/
/* Private function declaration ----------------------------------------------*/
void WriteFlashOneWord(uint32_t WriteAddress, uint32_t WriteData);
void WriteFlashData(uint32_t WriteAddress, uint8_t *data, uint32_t num);
void ReadFlashData(uint32_t ReadAddress, uint8_t *dest_Data, uint32_t num);
void Write_lut_ToData(int16_t *avg_pulse, uint8_t *data, uint16_t num);
void Write_Offset_ToData(uint16_t offset, uint8_t *data, uint16_t num);
void Read_lut_FromData(uint8_t *data, int16_t *lut_data, uint16_t num);
void Read_offset_FromData(uint8_t *data, uint16_t *offset, uint16_t num);
#endif
