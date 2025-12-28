/* Private includes ----------------------------------------------------------*/
#include "flash.h"
#include "stm32g4xx_hal.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void WriteFlashOneWord(uint32_t WriteAddress, uint32_t WriteData)
{
  HAL_FLASH_Unlock();  // 解锁 Flash
  uint64_t doubleWord = (uint64_t)WriteData | ((uint64_t)0xFFFFFFFF << 32);

  // 写入 64-bit 数据
  if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, STARTADDR + WriteAddress, doubleWord) != HAL_OK)
  {
    // 错误处理（如果需要）
  }

  HAL_FLASH_Lock();  // 锁定 Flash
}

void WriteFlashData(uint32_t WriteAddress, uint8_t *data, uint32_t num)
{
  HAL_FLASH_Unlock();  // 解锁 Flash

  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PageError = 0;

  // 计算起始页号
  uint32_t startPage = (STARTADDR + WriteAddress - FLASH_BASE) / FLASH_PAGE_SIZE;
  // 计算需要擦除的页数（向上取整）
  uint32_t pagesToErase = (num + (WriteAddress % FLASH_PAGE_SIZE) + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;

  // 计算擦除范围
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page      = startPage;
  EraseInitStruct.NbPages   = pagesToErase;  // 根据数据大小动态擦除页数

  HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);  // 擦除 Flash

  // 按 64-bit 对齐写入
  for(uint32_t i = 0; i < num; i += 8)
  {
    uint64_t doubleWord = 0xFFFFFFFFFFFFFFFF;
    memcpy(&doubleWord, &data[i], (num - i) < 8 ? (num - i) : 8);  // 复制数据到 64-bit 变量
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, STARTADDR + WriteAddress + i, doubleWord);
  }

  HAL_FLASH_Lock();  // 锁定 Flash
}

int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum)
{
  uint32_t realAddr = STARTADDR + ReadAddress;  // 计算实际地址
  memcpy(ReadBuf, (void *)realAddr, ReadNum);   // 直接拷贝数据
  return ReadNum;
}

void ReadFlashData(uint32_t ReadAddress, uint8_t *dest_Data, uint32_t num)
{
  uint32_t realAddr = STARTADDR + ReadAddress;  // 计算实际地址
  for(uint32_t i = 0; i < num; i++)
  {
    dest_Data[i] = *(__IO uint8_t *)(realAddr + i);  // 逐字节读取
  }
}

void Write_lut_ToData(int16_t *avg_pulse, uint8_t *data, uint16_t num)
{
  for(uint32_t i = 0; i < num; i++)
  {
    data[i * 2]     = avg_pulse[i] >> 8;  // 高 8 位
    data[i * 2 + 1] = avg_pulse[i];       // 低 8 位
  }
}

void Write_Offset_ToData(uint16_t offset, uint8_t *data, uint16_t num)
{
  data[num * 2]     = offset >> 8;    // 高 8 位
  data[num * 2 + 1] = offset & 0xFF;  // 低 8 位
}

void Read_lut_FromData(uint8_t *data, int16_t *lut_data, uint16_t num)
{
  for(uint32_t i = 0; i < num; i++)
  {
    lut_data[i] = (data[i * 2] << 8) | data[i * 2 + 1];  // 组合高 8 位和低 8 位
  }
}

void Read_offset_FromData(uint8_t *data, uint16_t *offset, uint16_t num)
{
  *offset = (data[num * 2] << 8) | data[num * 2 + 1];  // 组合高 8 位和低 8 位
}
