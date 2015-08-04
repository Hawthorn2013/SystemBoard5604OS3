/*
 * SDCard.h
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */

#pragma once


#define SDCARD_SECTOR_SIZE                              (512)
#define SDCARD_RES_OK                                   (0)
#define SDCARD_ERR_WRITE_BLOCK_FAILED                   (1)
#define SDCARD_OVERTIME_DEFAULT                         (1000)
#define SDCARD_OVERTIME_READ                            (100)
#define SDCARD_OVERTIME_WRITE                           (250)
#define SDCARD_OVERTIME_ERASE                           (250)
#define SDCARD_VERSION_1                                (10)
#define SDCARD_VERSION_2_STANDARD_CAPACITY              (21)
#define SDCARD_VERSION_2_HIGH_CAPACITY                  (22)
#define SDCARD_VERSION_UNUSABLE                         (0)


extern      int                 Init_SDCard(void);
extern      int                 SDCard_Read_Single_Block(uint32_t sector, uint8_t btffer[]);
extern      int                 SDCard_Read_Mult_Block(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt);
extern      int                 SDCard_Write_Single_Block(uint32_t sector, uint8_t buffer[]);
extern      int                 SDCard_Write_Mult_Blocks(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt);
