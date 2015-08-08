/*
 * SDCard.h
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */

#pragma once


#define SDCARD_SECTOR_SIZE                              (512)
#define SDCARD_BLOCK_SIZE                               (512)
#define SDCARD_ERR_WRITE_BLOCK_FAILED                   (1)
#define SDCARD_OVERTIME_DEFAULT                         (1000)
#define SDCARD_OVERTIME_READ                            (100)
#define SDCARD_OVERTIME_WRITE                           (250)
#define SDCARD_OVERTIME_ERASE                           (250)
#define SDCARD_VERSION_1                                (10)
#define SDCARD_VERSION_2_STANDARD_CAPACITY              (21)
#define SDCARD_VERSION_2_HIGH_CAPACITY                  (22)
#define SDCARD_VERSION_UNUSABLE                         (0)

typedef enum
{
    SDCARD_STATUS_READY = 0,
    SDCARD_STATUS_UNKNOWN,
    SDCARD_STATUS_UNINITIALIZED,
    SDCARD_STATUS_BUSY
} SDCARD_STATUS;

typedef enum
{
    SDCARD_RES_OK = 0,
    SDCARD_RES_ERR_DISK_UNREADY,
    SDCARD_RES_ERR_DISK_ERR,
    SDCARD_RES_ERR_BUS_UNREADY
} SDCARD_RES;

extern      SDCARD_RES          Init_SDCard(void);
extern      SDCARD_RES          SDCard_Read_Single_Block(uint32_t sector, uint8_t btffer[]);
extern      SDCARD_RES          SDCard_Read_Mult_Block(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt);
extern      SDCARD_RES          SDCard_Write_Single_Block(uint32_t sector, const uint8_t buffer[]);
extern      SDCARD_RES          SDCard_Write_Mult_Blocks(uint32_t sector, const uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt);
extern      SDCARD_STATUS       Get_SDCard_Status(void);
extern      SDCARD_RES          Get_SDCard_Sector_Count(uint32_t *cnt);
extern      SDCARD_RES          Get_SDCard_Sector_Size(uint16_t *cnt);
extern      SDCARD_RES          Get_SDCard_Block_Size(uint32_t *cnt);
