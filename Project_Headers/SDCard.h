/*
 * SDCard.h
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */

#pragma once


#define SDCARD_DSPI_PUSHR_CONT                          (1)
#define SDCARD_DSPI_PUSHR_PCS                           (DSPI_PUSHR_PCS_0)
#define SDCARD_DSPI_CTAR_DBR                            (0b0)
#define SDCARD_DSPI_CTAR_CPOL                           (0b0)
#define SDCARD_DSPI_CTAR_CPHA                           (0)
#define SDCARD_DSPI_CTAR_LSBFE                          (0b0)
#define SDCARD_DSPI_CTAR_PCSSCK                         (0b00)
#define SDCARD_DSPI_CTAR_PASC                           (0b00)
#define SDCARD_DSPI_CTAR_PDT                            (0b10)
#define SDCARD_DSPI_CTAR_PBR                            (0b00)
#define SDCARD_DSPI_CTAR_CSSCK                          (0b0111)
#define SDCARD_DSPI_CTAR_ASC                            (0b0111)
#define SDCARD_DSPI_CTAR_DT                             (0b0010)
#define SDCARD_DSPI_CTAR_BR                             (0b0110)
#define SDCARD_SECTOR_SIZE                              (512)
#define SDCARD_ERR_WRITE_BLOCK_FAILED                   (1)
#define SDCARD_OVERTIME_DEFAULT                         (1000)
#define SDCARD_OVERTIME_READ                            (100)
#define SDCARD_OVERTIME_WRITE                           (250)
#define SDCARD_OVERTIME_ERASE                           (250)
#define SDCARD_CMD0                                     (0)
#define SDCARD_CMD1                                     (1)
#define SDCARD_CMD8                                     (8)
#define SDCARD_CMD9                                     (9)
#define SDCARD_CMD12                                    (12)
#define SDCARD_CMD16                                    (16)
#define SDCARD_CMD17                                    (17)
#define SDCARD_CMD18                                    (18)
#define SDCARD_CMD24                                    (24)
#define SDCARD_CMD25                                    (25)
#define SDCARD_CMD55                                    (55)
#define SDCARD_CMD59                                    (59)
#define SDCARD_CMD58                                    (58)
#define SDCARD_ACMD41                                   (41)
#define SDCARD_VERSION_1                                (10)
#define SDCARD_VERSION_2_STANDARD_CAPACITY              (21)
#define SDCARD_VERSION_2_HIGH_CAPACITY                  (22)
#define SDCARD_VERSION_UNUSABLE                         (0)

typedef union
{
    uint8_t R[1];
    struct
    {
        uint8_t                 : 1;
        uint8_t Parameter_Err   : 1;
        uint8_t Address_Error   : 1;
        uint8_t Erase_Seq_Error : 1;
        uint8_t Com_CRC_Error   : 1;
        uint8_t Illegal_Command : 1;
        uint8_t Erase_Reset     : 1;
        uint8_t In_Idle_State   : 1;
    } B;
} R1;

typedef union
{
    uint8_t R[2];
    struct
    {
        R1 R1;
        uint8_t Out_of_Range_CSD_Overwrite              : 1;
        uint8_t Erase_Param                             : 1;
        uint8_t WP_Violation                            : 1;
        uint8_t Card_ECC_Failed                         : 1;
        uint8_t CC_Error                                : 1;
        uint8_t Error                                   : 1;
        uint8_t WP_Erase_Skip_Lock_Unlock_Cmd_Failed    : 1;
        uint8_t Card_is_Locked                          : 1;
    } B;
} R2;

typedef union
{
    uint8_t R[4];
    struct
    {
        uint8_t Card_power_up_status_bit    : 1;
        uint8_t CCS                         : 1;
        uint8_t                             : 6;
        uint8_t V3P5_3P6                    : 1;
        uint8_t V3P4_3P5                    : 1;
        uint8_t V3P3_3P4                    : 1;
        uint8_t V3P2_3P3                    : 1;
        uint8_t V3P1_3P2                    : 1;
        uint8_t V3P0_3P1                    : 1;
        uint8_t V2P9_3P0                    : 1;
        uint8_t V2P8_2P9                    : 1;
        uint8_t V2P7_2P8                    : 1;
        uint8_t                             : 7;
        uint8_t                             : 8;
    } B;
} OCR;

typedef union
{
    uint8_t R[5];
    struct
    {
        R1 R1;
        OCR OCR;
    } B;
} R3;

typedef struct
{
    struct DSPI_Device_Data *DSPI_dev;
    uint8_t CSD[16];
    int version;
} SDCard_Dev_Data;

typedef union
{
    uint8_t R[5];
    struct
    {
        R1 R1;
        uint8_t command_version     : 4;
        uint8_t                     : 4;
        uint8_t                     : 8;
        uint8_t                     : 4;
        uint8_t voltage_accepted    : 4;
        uint8_t check_pattern       : 8;
    } B;
} R7;


extern      int                 Set_DSPI_Device(struct DSPI_Device_Data *dspi);
extern      int                 Test_SDCard(void);
extern      int                 Reset_SDCard(void);
extern      int                 Test_SDCard_Read_Block(uint32_t sector, uint8_t btffer[]);
extern      int                 Test_SDCard_Read_Mult_Block(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt);
extern      int                 Test_SDCard_Write_Block(uint32_t sector, uint8_t buffer[]);
extern      int                 Test_SDCard_Write_Mult_Blocks(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt);
extern      int                 Get_SDCard_Size(uint32_t *size);
