/*
 * SDCard.c
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */


#define         __SDCARD_C_
#include        "includes.h"


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
    uint8_t R[16];
    union
    {
        struct
        {
            uint32_t CSD_STRUCTURE          : 2;
            uint32_t                        : 6;
            uint32_t TAAC                   : 8;
            uint32_t NSAC                   : 8;
            uint32_t TRAN_SPEED             : 8;
            uint32_t CCC                    : 12;
            uint32_t READ_BL_LEN            : 4;
            uint32_t READ_BL_PARTIAL        : 1;
            uint32_t WRITE_BLK_MISALIGN     : 1;
            uint32_t READ_BLK_MISALIGN      : 1;
            uint32_t DSR_IMP                : 1;
            uint32_t                        : 2;
            uint32_t C_SIZE_H               : 10;
            uint32_t C_SIZE_L               : 2;
            uint32_t VDD_R_CURR_MIN         : 3;
            uint32_t VDD_R_CURR_MAX         : 3;
            uint32_t VDD_W_CURR_MIN         : 3;
            uint32_t VDD_W_CURR_MAX         : 3;
            uint32_t C_SIZE_MULT            : 3;
            uint32_t ERASE_BLK_EN           : 1;
            uint32_t SECTOR_SIZE            : 7;
            uint32_t WP_GRP_SIZE            : 7;
            uint32_t WP_GRP_ENABLE          : 1;
            uint32_t                        : 2;
            uint32_t R2W_FACTOR             : 3;
            uint32_t WRITE_BL_LEN           : 4;
            uint32_t WRITE_BL_PARTIAL       : 1;
            uint32_t                        : 5;
            uint32_t FILE_FORMAT_GRP        : 1;
            uint32_t COPY                   : 1;
            uint32_t PERM_WRITE_PROTECT     : 1;
            uint32_t TMP_WRITE_PROTECT      : 1;
            uint32_t FILE_FORMAT            : 2;
            uint32_t                        : 2;
            uint32_t CRC                    : 7;
            uint32_t                        : 1;
        } CSD_V1_0;
        struct
        {
            uint32_t CSD_STRUCTURE          : 2;
            uint32_t                        : 6;
            uint32_t TAAC                   : 8;
            uint32_t NSAC                   : 8;
            uint32_t TRAN_SPEED             : 8;
            uint32_t CCC                    : 12;
            uint32_t READ_BL_LEN            : 4;
            uint32_t READ_BL_PARTIAL        : 1;
            uint32_t WRITE_BLK_MISALIGN     : 1;
            uint32_t READ_BLK_MISALIGN      : 1;
            uint32_t DSR_IMP                : 1;
            uint32_t                        : 6;
            uint32_t C_SIZE_H               : 6;
            uint32_t C_SIZE_L               : 16;
            uint32_t                        : 1;
            uint32_t ERASE_BLK_EN           : 1;
            uint32_t SECTOR_SIZE            : 7;
            uint32_t WP_GRP_SIZE            : 7;
            uint32_t WP_GRP_ENABLE          : 1;
            uint32_t                        : 2;
            uint32_t R2W_FACTOR             : 3;
            uint32_t WRITE_BL_LEN           : 4;
            uint32_t WRITE_BL_PARTIAL       : 1;
            uint32_t                        : 5;
            uint32_t FILE_FORMAT_GRP        : 1;
            uint32_t COPY                   : 1;
            uint32_t PERM_WRITE_PROTECT     : 1;
            uint32_t TMP_WRITE_PROTECT      : 1;
            uint32_t FILE_FORMAT            : 2;
            uint32_t                        : 2;
            uint32_t CRC                    : 7;
            uint32_t                        : 1;
        } CSD_V2_0;
    } B;
} CSD;

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
    int version;
    uint32_t sector_amount;
    CSD CSD;
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


static      uint8_t                         Test_SD_Buff[4][SDCARD_SECTOR_SIZE];
static      SDCard_Dev_Data                 SDCard_Dev_Data_1;


static      int                             Set_DSPI_Device(struct DSPI_Device_Data *dspi);
static      int                             Reset_SDCard(void);
static      int                             Rev_8_Bytes(uint8_t data[]);
static      int                             Send_8_Bytes(const uint8_t data[]);
static      int                             Send_8_Clocks_withoout_CS(void);
static      int                             Send_80_Clocks_without_CS(void);
static      void                            Send_Byte(uint8_t data);
static      uint8_t                         Rev_Byte(void);
static      int                             Get_CSD(void);
static      int                             Test_SDCard_Send_Cmd(int cmd, uint32_t var, uint8_t crc, uint8_t *rev, int len);
static      uint8_t                         Cal_CRC7(uint8_t *data, int cnt);
static      int                             Get_SDCard_Size(void);


static int Set_DSPI_Device(struct DSPI_Device_Data *dspi)
{
    SDCard_Dev_Data_1.DSPI_dev = dspi;
    return 0;
}


SDCARD_RES Init_SDCard(void)
{
    Set_DSPI_Device(&DSPI_1_Device_Data);
    if (DSPI_RES_OK != Open_DSPI_Dev(SDCard_Dev_Data_1.DSPI_dev))
    {
        return SDCARD_RES_ERR_BUS_UNREADY;
    }
    Set_DSPI_CTAR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_CTAR_DBR, SDCARD_DSPI_CTAR_CPOL, SDCARD_DSPI_CTAR_CPHA, SDCARD_DSPI_CTAR_LSBFE, SDCARD_DSPI_CTAR_PCSSCK, SDCARD_DSPI_CTAR_PASC, SDCARD_DSPI_CTAR_PDT, SDCARD_DSPI_CTAR_PBR, SDCARD_DSPI_CTAR_CSSCK, SDCARD_DSPI_CTAR_ASC, SDCARD_DSPI_CTAR_DT, SDCARD_DSPI_CTAR_BR);
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    Reset_SDCard();
    Get_CSD();
    Get_SDCard_Size();
    Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
    return SDCARD_RES_OK;
}


SDCARD_RES Get_SDCard_Sector_Count(uint32_t *cnt)
{
    SDCARD_RES res;
    
    if (SDCard_Dev_Data_1.version != SDCARD_VERSION_UNUSABLE)
    {
        *cnt = SDCard_Dev_Data_1.sector_amount;
        res = SDCARD_RES_OK;
    }
    else
    {
        *cnt = 0;
        res = SDCARD_RES_ERR_DISK_UNREADY;
    }
    return res;
}


SDCARD_RES Get_SDCard_Sector_Size(uint16_t *cnt)
{
    SDCARD_RES res;
    
    if (SDCard_Dev_Data_1.version != SDCARD_VERSION_UNUSABLE)
    {
        *cnt = SDCARD_SECTOR_SIZE;
        res = SDCARD_RES_OK;
    }
    else
    {
        *cnt = 0;
        res = SDCARD_RES_ERR_DISK_UNREADY;
    }
    return res;
}


SDCARD_RES Get_SDCard_Block_Size(uint32_t *cnt)
{
    SDCARD_RES res;
    
    if (SDCard_Dev_Data_1.version != SDCARD_VERSION_UNUSABLE)
    {
        *cnt = SDCARD_BLOCK_SIZE;
        res = SDCARD_RES_OK;
    }
    else
    {
        *cnt = 0;
        res = SDCARD_RES_ERR_DISK_UNREADY;
    }
    return res;
}


static int Test_SDCard_Send_Cmd(int cmd, uint32_t var, uint8_t crc, uint8_t *rev, int len)
{
    uint8_t send[6], res = 0;
    
    send[0] = (uint8_t)(cmd | 0x40);
    *(uint32_t *)&(send[1]) = var;
    send[5] = crc;
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 6);
    do
    {
        res = Rev_Byte();
    }
    while (0xFF == res);
    *(rev++) = res;
    while(--len)
    {
        *(rev++) = Rev_Byte();
    }
    Send_8_Clocks_withoout_CS();
}


static int Reset_SDCard(void)
{
    R7 r7;
    R1 r1;
    R3 r3;
    uint8_t crc = 0x00;
    uint8_t data[5] = {0x48, 0x00, 0x00, 0x01, 0xAA};
    
    crc = Cal_CRC7(data, sizeof(data));
    Send_80_Clocks_without_CS();
    do
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD0, 0, 0x95, r1.R, sizeof(r1));
    }
    while (0x01 != r1.R[0]);

    while(1)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD8, 0x000001AA, crc, r7.R, sizeof(r7));
        if (1 == r7.B.R1.B.Illegal_Command)
        {
            SDCard_Dev_Data_1.version = SDCARD_VERSION_1;
            break;
        }
        if (0xAA != r7.B.check_pattern)
        {
            continue;
        }
        if (0b0001 != r7.B.voltage_accepted)
        {
            SDCard_Dev_Data_1.version = SDCARD_VERSION_UNUSABLE;
            return 1;
        }
        else
        {
            break;
        }
    }
    while(1)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD58, 0x00000000, 0xFF, r3.R, sizeof(r3));
        if (0x01 != r3.B.R1.R[0])
        {
            continue;
        }
        if ((!r3.B.OCR.B.V3P2_3P3) || (!r3.B.OCR.B.V3P3_3P4))
        {
            SDCard_Dev_Data_1.version = SDCARD_VERSION_UNUSABLE;
            return 2;
        }
        else
        {
            break;
        }
    }
    
    while(1)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD55, 0x00000000, 0xFF, r1.R, sizeof(r1));
        if (0x01 != r1.R[0])
        {
            continue;
        }
        Test_SDCard_Send_Cmd(SDCARD_ACMD41, 0x40000000, 0xFF, r1.R, sizeof(r1));
        if (0x00 == r1.R[0])
        {
            break;
        }
    }
    
    while(1)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD58, 0x00000000, 0xFF, r3.R, sizeof(r3));
        if (0x00 != r3.B.R1.R[0])
        {
            continue;
        }
        if (!r3.B.OCR.B.Card_power_up_status_bit)
        {
            SDCard_Dev_Data_1.version = SDCARD_VERSION_UNUSABLE;
            return 3;
        }
        if (r3.B.OCR.B.CCS)
        {
            SDCard_Dev_Data_1.version = SDCARD_VERSION_2_HIGH_CAPACITY;
            break;
        }
        else
        {
            SDCard_Dev_Data_1.version = SDCARD_VERSION_2_STANDARD_CAPACITY;
            break;
        }
    }
    while(1)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD59, 0, 0xFF, r1.R, sizeof(r1));
        if (0x00 == r1.R[0])
        {
            break;
        }
    }
    while(1)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD16, 512, 0xFF, r1.R, sizeof(r1));
        if (0x00 == r1.R[0])
        {
            break;
        }
    }
    return 0;
}


SDCARD_RES SDCard_Read_Single_Block(uint32_t sector, uint8_t buffer[])
{
    R1 r1;
    int i = 0;
    uint8_t send[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    
    if (DSPI_RES_OK != Open_DSPI_Dev(SDCard_Dev_Data_1.DSPI_dev))
    {
        return SDCARD_RES_ERR_BUS_UNREADY;
    }
    if (SDCARD_VERSION_2_STANDARD_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD17, sector<<9, 0xFF, r1.R, sizeof(r1));
    }
    else if (SDCARD_VERSION_2_HIGH_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD17, sector, 0xFF, r1.R, sizeof(r1));
    }
    else
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_UNREADY;
    }
    if (0x00 != r1.R[0])
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_ERR;
    }
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    while(1)
    {
        uint8_t res = 0x00;

        res = Rev_Byte();
        if (0xFE == res)
        {
            break;
        }
    }
    for (i = 0; i < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; i++)
    {
        Rev_8_Bytes(buffer + DSPI_ASYNC_SEND_DATA_MAX_LENGTH * i);
    }
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 2);
    Send_8_Clocks_withoout_CS();
    Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
    return SDCARD_RES_OK;
}


SDCARD_RES SDCard_Read_Mult_Block(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt)
{
    R1 r1;
    int i = 0, j = 0;
    uint8_t send[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    
    if (DSPI_RES_OK != Open_DSPI_Dev(SDCard_Dev_Data_1.DSPI_dev))
    {
        return SDCARD_RES_ERR_BUS_UNREADY;
    }
    if (SDCARD_VERSION_2_STANDARD_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD18, sector<<9, 0xFF, r1.R, sizeof(r1));
    }
    else if(SDCARD_VERSION_2_HIGH_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD18, sector, 0xFF, r1.R, sizeof(r1));
    }
    else
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_UNREADY;
    }
    if (0x00 != r1.R[0])
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_ERR;
    }
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    
    for (i = 0; i < block_cnt; i++)
    {
        while(1)
        {
            uint8_t res = 0x00;
            
            res = Rev_Byte();
            if (0xFE == res)
            {
                break;
            }
        }
        for (j = 0; j < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; j++)
        {
            Rev_8_Bytes(&buffer[i][DSPI_ASYNC_SEND_DATA_MAX_LENGTH * j]);
        }
        DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 2);
    }
    Test_SDCard_Send_Cmd(SDCARD_CMD12, 0, 0xFF, r1.R, sizeof(r1));
    Send_8_Clocks_withoout_CS();
    Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
    return SDCARD_RES_OK;
}


SDCARD_RES SDCard_Write_Single_Block(uint32_t sector, const uint8_t buffer[])
{
    R1 r1;
    int i = 0;
    uint8_t send[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, };
    uint8_t res = 0x00;
    
    if (DSPI_RES_OK != Open_DSPI_Dev(SDCard_Dev_Data_1.DSPI_dev))
    {
        return SDCARD_RES_ERR_BUS_UNREADY;
    }
    if (SDCARD_VERSION_2_STANDARD_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD24, sector<<9, 0xFF, r1.R, sizeof(r1));
    }
    else if (SDCARD_VERSION_2_HIGH_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD24, sector, 0xFF, r1.R, sizeof(r1));
    }
    else
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_UNREADY;
    }
    if (0x00 != r1.R[0])
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_ERR;
    }
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send + 5, NULL, 4);
    for (i = 0; i < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; i++)
    {
        Send_8_Bytes(&buffer[DSPI_ASYNC_SEND_DATA_MAX_LENGTH * i]);
    }
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 2);
    res = Rev_Byte();
    if ((res & 0b00011111) != 0b00000101)
    {
        return SDCARD_RES_ERR_DISK_ERR;
    }
    while(1)
    {
        uint8_t res = 0x00;

        res = Rev_Byte();
        if (0xFF == res)
        {
            break;
        }
    }
    Send_8_Clocks_withoout_CS();
    Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
    return SDCARD_RES_OK;
}


SDCARD_RES SDCard_Write_Mult_Blocks(uint32_t sector, const uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt)
{
    R1 r1;
    int i = 0, j = 0;
    uint8_t send[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFD};
    uint8_t res = 0x00;
    

    if (DSPI_RES_OK != Open_DSPI_Dev(SDCard_Dev_Data_1.DSPI_dev))
    {
        return SDCARD_RES_ERR_BUS_UNREADY;
    }
    if (SDCARD_VERSION_2_STANDARD_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD25, sector<<9, 0xFF, r1.R, sizeof(r1));
    }
    else if (SDCARD_VERSION_2_HIGH_CAPACITY == SDCard_Dev_Data_1.version)
    {
        Test_SDCard_Send_Cmd(SDCARD_CMD25, sector, 0xFF, r1.R, sizeof(r1));
    }
    else
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_UNREADY;
    }
    if (0x00 != r1.R[0])
    {
        Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
        return SDCARD_RES_ERR_DISK_ERR;
    }
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 3);
    for (j = 0; j < block_cnt; j++)
    {
        DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send + 8, NULL, 1);
        for (i = 0; i < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; i++)
        {
            Send_8_Bytes(&buffer[j][DSPI_ASYNC_SEND_DATA_MAX_LENGTH * i]);
        }
        DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 2);
        res = Rev_Byte();
        if ((res & 0b00011111) != 0b00000101)
        {
            Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
            return SDCARD_ERR_WRITE_BLOCK_FAILED;
        }
        while(1)
        {
            uint8_t res = 0x00;
            
            res = Rev_Byte();
            if (0xFF == res)
            {
                break;
            }
        }
    }
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send + 9, NULL, 1);
    while(1)
    {
        uint8_t res = 0x00;
        
        res = Rev_Byte();
        if (0xFF == res)
        {
            break;
        }
    }
    Send_8_Clocks_withoout_CS();
    Close_DSPI(SDCard_Dev_Data_1.DSPI_dev);
    return SDCARD_RES_OK;
}


SDCARD_STATUS Get_SDCard_Status(void)
{
    switch (SDCard_Dev_Data_1.version)
    {
    case SDCARD_VERSION_1 :
    case SDCARD_VERSION_2_STANDARD_CAPACITY :
    case SDCARD_VERSION_2_HIGH_CAPACITY :
        return SDCARD_STATUS_READY;
        break;
    case SDCARD_VERSION_UNUSABLE :
    default :
        return SDCARD_STATUS_UNKNOWN;
        break;
    }
}


static int Rev_8_Bytes(uint8_t data[])
{
    uint8_t send[DSPI_ASYNC_SEND_DATA_MAX_LENGTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    int rev;
    
    rev = DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, data, DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
    return rev;
}


static int Send_8_Clocks_withoout_CS(void)
{
    uint8_t send = 0xFF;
    
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, DSPI_PUSHR_PCS_NONE);
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, &send, NULL, 1);
}


static int Send_80_Clocks_without_CS(void)
{
    uint8_t send[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, };
    
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, DSPI_PUSHR_PCS_NONE);
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 8);
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, send, NULL, 2);
}


static int Send_8_Bytes(const uint8_t data[])
{
    int rev;
    
    rev = DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, data, NULL, DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
    return rev;
}


static void Send_Byte(uint8_t data)
{
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, &data, NULL, 1);
}


static uint8_t Rev_Byte(void)
{
    uint8_t send = 0xFF, rev = 0x00;
    
    DSPI_SYNC_Send_and_Receive_Data(SDCard_Dev_Data_1.DSPI_dev, &send, &rev, 1);
    return rev;
}


static int Get_CSD(void)
{
    R1 r1;
    
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    Test_SDCard_Send_Cmd(SDCARD_CMD9, 0x00000000, 0xFF, r1.R, sizeof(r1));
    if (0x00 != r1.R[0])
    {
        return (int)r1.R[0];
    }
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    while(1)
    {
        uint8_t res = 0x00;

        res = Rev_Byte();
        if (0xFE == res)
        {
            break;
        }
    }
    Rev_8_Bytes(SDCard_Dev_Data_1.CSD.R);
    Rev_8_Bytes(SDCard_Dev_Data_1.CSD.R + 8);
    Rev_8_Bytes(NULL);
    Rev_8_Bytes(NULL);
    Send_8_Clocks_withoout_CS();
    return 0;
}


static int Get_SDCard_Size(void)
{
    
    if (SDCARD_VERSION_2_STANDARD_CAPACITY == SDCard_Dev_Data_1.version)
    {
        uint32_t block_len = 1024;
        uint32_t c_size_mult = SDCard_Dev_Data_1.CSD.B.CSD_V1_0.C_SIZE_MULT;
        uint32_t c_size = 0x00000000;
        c_size |= (uint32_t)(SDCard_Dev_Data_1.CSD.B.CSD_V1_0.C_SIZE_H) << 2;
        c_size |= (uint32_t)(SDCard_Dev_Data_1.CSD.B.CSD_V1_0.C_SIZE_L);
        SDCard_Dev_Data_1.sector_amount = (c_size + 1) * ((uint32_t)0x00000001 << (c_size_mult + 2)) * (block_len / 512);
    }
    else if (SDCARD_VERSION_2_HIGH_CAPACITY == SDCard_Dev_Data_1.version)
    {
        uint32_t c_size = 0x00000000;
        c_size |= (uint32_t)(SDCard_Dev_Data_1.CSD.B.CSD_V2_0.C_SIZE_H) << 16;
        c_size |= (uint32_t)(SDCard_Dev_Data_1.CSD.B.CSD_V2_0.C_SIZE_L);
        SDCard_Dev_Data_1.sector_amount = c_size;
    }
    else
    {
        return 1;
    }
    return 0;
}


static uint8_t Cal_CRC7(uint8_t *data, int cnt)
{
    int i, a;
    unsigned char crc = 0x00, Data;
    
    for (a=0; a < cnt; a++)
    {
        Data = data[a];
        for (i = 0; i < 8; i++)
        {
            crc <<= 1;
            if ((Data & 0x80) ^ (crc & 0x80))
            {
                crc ^= 0x09;
            }
            Data <<= 1;
        }
    }
    crc = (crc << 1) | 1;
    return crc;
}
