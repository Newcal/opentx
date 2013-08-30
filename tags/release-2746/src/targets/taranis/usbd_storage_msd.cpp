/**
  ******************************************************************************
  * @file    usbd_storage_msd.c
  * @author  MCD application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the disk operations functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "../opentx.h"
#include "../FatFs/diskio.h"
#include "board_taranis.h"

extern "C" {
#include "STM32F2xx_StdPeriph_Lib_V1.1.0/Libraries/STM32F2xx_StdPeriph_Driver/inc/misc.h"
#include "usbd_msc_mem.h"
#include "usb_conf.h"

void eeprom_read_block (void *pointer_ram, uint16_t pointer_eeprom, size_t size);

#define STORAGE_LUN_NBR    2
#define BLOCKSIZE          512

/* USB Mass storage Standard Inquiry Data */
const unsigned char STORAGE_Inquirydata[] = {//36
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (USBD_STD_INQUIRY_LENGTH - 5),
  0x00,
  0x00,	
  0x00,
  'F', 'r', 'S', 'k', 'y', ' ', ' ', ' ',  /* Manufacturer : 8 bytes */
  'T', 'a', 'r', 'a', 'n', 'i', 's', ' ',  /* Product      : 16 Bytes */
  'R', 'a', 'd', 'i', 'o', ' ', ' ', ' ',
  '1', '.', '0', '0',                      /* Version      : 4 Bytes */
  /* LUN 1 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (USBD_STD_INQUIRY_LENGTH - 5),
  0x00,
  0x00,	
  0x00,
  'F', 'r', 'S', 'k', 'y', ' ', ' ', ' ',  /* Manufacturer : 8 bytes */
  'T', 'a', 'r', 'a', 'n', 'i', 's', ' ',  /* Product      : 16 Bytes */
  'R', 'a', 'd', 'i', 'o', ' ', ' ', ' ',
  '1', '.', '0' ,'0',                      /* Version      : 4 Bytes */
}; 

int32_t fat12Write( const uint8_t *buffer, uint16_t sector, uint32_t count ) ;
int32_t fat12Read( uint8_t *buffer, uint16_t sector, uint16_t count ) ;

int8_t STORAGE_Init (uint8_t lun);

int8_t STORAGE_GetCapacity (uint8_t lun, 
                           uint32_t *block_num, 
                           uint32_t *block_size);

int8_t  STORAGE_IsReady (uint8_t lun);

int8_t  STORAGE_IsWriteProtected (uint8_t lun);

int8_t STORAGE_Read (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_Write (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_GetMaxLun (void);

USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata,
};

USBD_STORAGE_cb_TypeDef  *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;

__IO uint32_t count = 0;
}

int8_t STORAGE_Init (uint8_t lun)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

/* TODO if no SD ... if( SD_Init() != 0)
  {
    return (-1); 
  } 
*/
  return (0);
}

/**
  * @brief  return medium capacity and block size
  * @param  lun : logical unit number
  * @param  block_num :  number of physical block
  * @param  block_size : size of a physical block
  * @retval Status
  */
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
  if (lun == 1)	{
    *block_size = BLOCKSIZE;
    *block_num  = EESIZE/BLOCKSIZE + 3 ;
  }
  else {
    if (!SD_CARD_PRESENT())
      return -1;
  
    *block_size = BLOCKSIZE;

    static DWORD sector_count = 0;
    if (sector_count == 0) {
      if (disk_ioctl(0, GET_SECTOR_COUNT, &sector_count) != RES_OK) {
        sector_count = 0;
        return -1;
      }
    }

    *block_num  = sector_count;
  }

  return 0;
}

/**
  * @brief  check whether the medium is ready
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsReady (uint8_t lun)
{ 
  if (lun == 1) {
    return 0;
  }
  else {
    return SD_CARD_PRESENT() ? 0 : -1;
  }
}

/**
  * @brief  check whether the medium is write-protected
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
  return  0;
}

/**
  * @brief  Read data from the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to save data
  * @param  blk_addr :  address of 1st block to be read
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */

int8_t SD_ReadSectors(uint8_t *buff, uint32_t sector, uint32_t count);

int8_t STORAGE_Read (uint8_t lun, 
                 uint8_t *buf, 
                 uint32_t blk_addr,                       
                 uint16_t blk_len)
{
  if (lun == 1) {
    if (fat12Read(buf, blk_addr, blk_len) != 0) {
      return -1;
    }
  }
  else {
    if (SD_ReadSectors(buf,
                       blk_addr,
                       blk_len) != 0) {
      return -1;
    }
  }

  return 0;
}
/**
  * @brief  Write data to the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to write from
  * @param  blk_addr :  address of 1st block to be written
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */

int8_t SD_WriteSectors(uint8_t *buf, uint32_t sector, uint32_t count);

int8_t STORAGE_Write (uint8_t lun, 
                  uint8_t *buf, 
                  uint32_t blk_addr,
                  uint16_t blk_len)
{
  if (lun == 1)	{
    if (fat12Write(buf, blk_addr, blk_len) != 0)
      return -1;
  }
  else {
    if (SD_WriteSectors(buf,
                        blk_addr,
                        blk_len) != 0)
      return -1;
  }

  return (0);
}

/**
  * @brief  Return number of supported logical unit
  * @param  None
  * @retval number of logical unit
  */

int8_t STORAGE_GetMaxLun (void)
{
  return (STORAGE_LUN_NBR - 1);
}


//------------------------------------------------------------------------------
/**
 * FAT12 boot sector partition.
 */
const char g_FATboot[BLOCKSIZE] =
{
    0xeb, 0x3c, 0x90, // Jump instruction.
    0x39, 0x58, 0x20, 0x54, 0x45, 0x41, 0x4D, 0x00, // OEM Name
    0x00, 0x02, // Bytes per sector
    0x01, // Sectors per FS cluster.
    0x01, 0x00, // Reserved sector count
    
    0x01, // Number of FATs
    0x10, 0x00, // Number of root directory entries
    (EESIZE/BLOCKSIZE)+3, 0x00, // Total sectors = 131
    0xf8, // Media descriptor
    0x01, 0x00, // Sectors per FAT table
    0x20, 0x00, // Sectors per track
    0x40, 0x00, // Number of heads
    0x00, 0x00, 0x00, 0x00, // Number of hidden sectors
    
    0x00, 0x00, 0x00, 0x00, // Large number of sectors.
    0x00, // Physical drive number
    0x00, // Reserved
    0x29, // Extended boot signature
    0xCE, 0xFA, 0x5C, 0xD1, // Disk ID (serial number)
    0x56, 0x49, 0x52, 0x54, 0x46, 0x53, 0x20, 0x20, 0x20, 0x20, 0x20, // Volume label
    0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20, // FAT file system type
    0x00, 0x00, // OS boot code
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
};

#if defined(REV4a)
const char g_FAT[BLOCKSIZE] =
{
    0xF8, 0xFF, 0xFF, 0x03, 0x40, 0x00, 0x05, 0x60, 0x00, 0x07, 0x80, 0x00, 0x09, 0xA0, 0x00, 0x0B,
    0xC0, 0x00, 0x0D, 0xE0, 0x00, 0x0F, 0x00, 0x01, 0x11, 0x20, 0x01, 0x13, 0x40, 0x01, 0x15, 0x60,
    0x01, 0x17, 0x80, 0x01, 0x19, 0xA0, 0x01, 0x1B, 0xC0, 0x01, 0x1D, 0xE0, 0x01, 0x1F, 0x00, 0x02,
    0x21, 0x20, 0x02, 0x23, 0x40, 0x02, 0x25, 0x60, 0x02, 0x27, 0x80, 0x02, 0x29, 0xA0, 0x02, 0x2B,
    0xC0, 0x02, 0x2D, 0xE0, 0x02, 0x2F, 0x00, 0x03, 0x31, 0x20, 0x03, 0x33, 0x40, 0x03, 0x35, 0x60,
    0x03, 0x37, 0x80, 0x03, 0x39, 0xA0, 0x03, 0x3B, 0xC0, 0x03, 0x3D, 0xE0, 0x03, 0x3F, 0x00, 0x04,
    0x41, 0x20, 0x04, 0x43, 0x40, 0x04, 0x45, 0x60, 0x04, 0x47, 0x80, 0x04, 0x49, 0xA0, 0x04, 0x4B,
    0xC0, 0x04, 0x4D, 0xE0, 0x04, 0x4F, 0x00, 0x05, 0x51, 0x20, 0x05, 0x53, 0x40, 0x05, 0x55, 0x60,
    0x05, 0x57, 0x80, 0x05, 0x59, 0xA0, 0x05, 0x5B, 0xC0, 0x05, 0x5D, 0xE0, 0x05, 0x5F, 0x00, 0x06,
    0x61, 0x20, 0x06, 0x63, 0x40, 0x06, 0x65, 0x60, 0x06, 0x67, 0x80, 0x06, 0x69, 0xA0, 0x06, 0x6B,
    0xC0, 0x06, 0x6D, 0xE0, 0x06, 0x6F, 0x00, 0x07, 0x71, 0x20, 0x07, 0x73, 0x40, 0x07, 0x75, 0x60,
    0x07, 0x77, 0x80, 0x07, 0x79, 0xA0, 0x07, 0x7B, 0xC0, 0x07, 0x7D, 0xE0, 0x07, 0x7F, 0x00, 0x08,
    0x81, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#else
const char g_FAT[BLOCKSIZE] =
{
    0xF8, 0xFF, 0xFF, 0x03, 0x40, 0x00, 0x05, 0x60, 0x00, 0x07, 0x80, 0x00, 0x09, 0xA0, 0x00, 0x0B,
    0xC0, 0x00, 0x0D, 0xE0, 0x00, 0x0F, 0x00, 0x01, 0x11, 0x20, 0x01, 0x13, 0x40, 0x01, 0x15, 0x60,
    0x01, 0x17, 0x80, 0x01, 0x19, 0xA0, 0x01, 0x1B, 0xC0, 0x01, 0x1D, 0xE0, 0x01, 0x1F, 0x00, 0x02,
    0x21, 0x20, 0x02, 0x23, 0x40, 0x02, 0x25, 0x60, 0x02, 0x27, 0x80, 0x02, 0x29, 0xA0, 0x02, 0x2B,
    0xC0, 0x02, 0x2D, 0xE0, 0x02, 0x2F, 0x00, 0x03, 0x31, 0x20, 0x03, 0x33, 0x40, 0x03, 0x35, 0x60,
    0x03, 0x37, 0x80, 0x03, 0x39, 0xA0, 0x03, 0x3B, 0xC0, 0x03, 0x3D, 0xE0, 0x03, 0x3F, 0x00, 0x04,
    0x41, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

//	File Attributes
//	0 	0x01 	Read Only.
//	1 	0x02 	Hidden.
//	2 	0x04 	System.
//	3 	0x08 	Volume Label.
//	4 	0x10 	Subdirectory.
//	5 	0x20 	Archive.
//	6 	0x40 	Device.
//	7 	0x80 	Reserved.

typedef struct
{
    uint8_t name[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t create_time_ms;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t ea_index;
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t start_cluster;
    uint32_t file_size;
} FATDirEntry_t;

// First 16 FAT root directory entries (1 sector)
const FATDirEntry_t g_DIRroot[16] =
{
    {
        { 'T', 'A', 'R', 'A', 'N', 'I', 'S', ' '},
        { ' ', ' ', ' '},
        0x08,		// Volume
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { 'T', 'A', 'R', 'A', 'N', 'I', 'S', ' '},
        { 'B', 'I', 'N'},
        0x24,		// Archive, hidden, system
        0x00,
        0x3E,
        0xA301,
        0x3D55,
        0x3D55,
        0x0000,
        0xA302,
        0x3D55,
        0x0002,
        EESIZE
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
    {
        { '\x00', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        { ' ', ' ', ' '},
        0x00,
        0x00,
        0x00,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        0x00000000
    },
};

// count is number of 512 byte sectors
int32_t fat12Read( uint8_t *buffer, uint16_t sector, uint16_t count )
{
  while ( count )
  {
    if (sector == 0) {
      memcpy( buffer, g_FATboot, BLOCKSIZE ) ;
    }
    else if (sector == 1/*Reserved sector count*/) {
      // FAT table.
      memcpy( buffer, g_FAT, BLOCKSIZE);
    }
    else if (sector == 2) {
      memcpy( buffer, g_DIRroot, BLOCKSIZE ) ;
    }
    else {
      eeprom_read_block (buffer, (sector-3)*BLOCKSIZE, BLOCKSIZE);
    }
    buffer += BLOCKSIZE ;
    sector++ ;
    count-- ;
  }
  return 0 ;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int32_t fat12Write(const uint8_t *buffer, uint16_t sector, uint32_t count )
{
  static int offset = 0;

  TRACE("FAT12 Write(sector=%d, count=%d)", sector, count);

  while (count) {
    const EeFs * test = (const EeFs *)buffer;
    if (offset == 0 && test->version==EEFS_VERS && test->mySize==sizeof(eeFs) && test->bs==BS) {
      TRACE("EEPROM start found in sector %d", sector);
      offset = sector;
    }
    if (offset && sector >= offset && (sector-offset) < EESIZE/BLOCKSIZE) {
      eeWriteBlockCmp((uint8_t *)buffer, (sector-offset)*BLOCKSIZE, BLOCKSIZE);
    }
    buffer += BLOCKSIZE;
    sector++;
    count--;
    if (sector-offset >= EESIZE/BLOCKSIZE) {
      TRACE("EEPROM end written at sector %d", sector-1);
      offset = 0;
    }
  }
  return 0 ;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

