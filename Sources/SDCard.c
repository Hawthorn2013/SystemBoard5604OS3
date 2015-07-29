/*
 * SDCard.c
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */


#include        "includes.h"


static      uint8_t                         Test_SD_Buff[4][SDCARD_SECTOR_SIZE];
static      SDCard_Dev_Data                 SDCard_Dev_Data_1;

static      int                             Test_SDCard_Send_Cmd(int cmd, uint32_t var, uint8_t crc, uint32_t *rev);
static      int                             Rev_8_Bytes(uint8_t data[]);
static      int                             Send_8_Bytes(uint8_t data[]);
static      int                             Send_8_Clocks_withoout_CS(void);
static      int                             Send_80_Clocks_without_CS(void);
static      void                            Send_Byte(uint8_t data);
static      uint8_t                         Rev_Byte(void);
static      int                             Test_Get_CSD(void);
static      int                             Test_SDCard_Send_Cmd_Ex(int cmd, uint32_t var, uint8_t crc, uint8_t *rev);
static      uint8_t                         Cal_CRC7(uint8_t *data, int cnt);


int Set_DSPI_Device(struct DSPI_Device_Data *dspi)
{
    SDCard_Dev_Data_1.DSPI_dev = dspi;
    return 0;
}


int Test_SDCard(void)
{
    int rev = 0;
    uint32_t size = 0;
    
    Set_DSPI_Device(&DSPI_1_Device_Data);
    Open_DSPI_Dev(SDCard_Dev_Data_1.DSPI_dev);
    Set_DSPI_CTAR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_CTAR_DBR, SDCARD_DSPI_CTAR_CPOL, SDCARD_DSPI_CTAR_CPHA, SDCARD_DSPI_CTAR_LSBFE, SDCARD_DSPI_CTAR_PCSSCK, SDCARD_DSPI_CTAR_PASC, SDCARD_DSPI_CTAR_PDT, SDCARD_DSPI_CTAR_PBR, SDCARD_DSPI_CTAR_CSSCK, SDCARD_DSPI_CTAR_ASC, SDCARD_DSPI_CTAR_DT, SDCARD_DSPI_CTAR_BR);
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    Reset_SDCard();
    Get_SDCard_Size(&size);
    
    do
    {
        rev = Test_SDCard_Read_Block(0, Test_SD_Buff[0]);
    }
    while(rev);
    
    do
    {
        rev = Test_SDCard_Read_Block(1, Test_SD_Buff[1]);
    }
    while(rev);
    
    do
    {
        rev = Test_SDCard_Write_Mult_Blocks(4, (uint8_t (*)[SDCARD_SECTOR_SIZE])(Test_SD_Buff[0]), 2);
    }
    while(rev);
    
    do
    {
        rev = Test_SDCard_Read_Block(4, Test_SD_Buff[2]);
    }
    while(rev);
    
    do
    {
        rev = Test_SDCard_Read_Block(5, Test_SD_Buff[3]);
    }
    while(1);
}


static int Test_SDCard_Send_Cmd(int cmd, uint32_t var, uint8_t crc, uint32_t *rev)
{
    uint32_t sector = 0x00000000;
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
    *rev = res;
    Send_8_Clocks_withoout_CS();
}


static int Test_SDCard_Send_Cmd_Ex(int cmd, uint32_t var, uint8_t crc, uint8_t *rev)
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
    switch (cmd)
    {
    case SDCARD_CMD8 :
        *(rev++) = Rev_Byte();
        *(rev++) = Rev_Byte();
        *(rev++) = Rev_Byte();
        *(rev++) = Rev_Byte();
        break;
    default :
        break;
    }
    Send_8_Clocks_withoout_CS();
}


int Reset_SDCard(void)
{
    uint32_t rev = 0x00000000;
    R7 r7;
    uint8_t crc = 0x00;
    uint8_t data[5] = {0x40, 0x00, 0x00, 0x00, 0x00};
    
    crc = Cal_CRC7(data, sizeof(data));
    Send_80_Clocks_without_CS();
    do
    {
        Test_SDCard_Send_Cmd(0, 0, 0x95, &rev);
    }
    while (0x01 != rev);
    
    Test_SDCard_Send_Cmd_Ex(SDCARD_CMD8, 0x000000AA, 0xFF, r7);
    
    do
    {
        Test_SDCard_Send_Cmd(1, 0, 0xFF, &rev);
    }
    while (0x00 != rev);
    Test_SDCard_Send_Cmd(59, 0, 0xFF, &rev);
    Test_SDCard_Send_Cmd(16, 512, 0xFF, &rev);
    Test_Get_CSD();
    return 0;
}


int Test_SDCard_Read_Block(uint32_t sector, uint8_t buffer[])
{
    uint32_t rev = 0x00000000;
    int i = 0;
    uint8_t send[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    
    Test_SDCard_Send_Cmd(17, sector<<9, 0xFF, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
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
    return 0;
}


int Test_SDCard_Read_Mult_Block(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt)
{
    uint32_t rev = 0x00000000;
    int i = 0, j = 0;
    uint8_t send[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    
    Test_SDCard_Send_Cmd(18, sector<<9, 0xFF, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
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
    Test_SDCard_Send_Cmd(12, 0, 0xFF, &rev);
    Send_8_Clocks_withoout_CS();
    return 0;
}


int Test_SDCard_Write_Block(uint32_t sector, uint8_t buffer[])
{
    uint32_t rev = 0x00000000;
    int i = 0;
    uint8_t send[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, };
    uint8_t res = 0x00;
    
    Test_SDCard_Send_Cmd(24, sector<<9, 0xFF, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
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
    Send_8_Clocks_withoout_CS();
    return 0;
}


int Test_SDCard_Write_Mult_Blocks(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt)
{
    uint32_t rev = 0x00000000;
    int i = 0, j = 0;
    uint8_t send[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFD};
    uint8_t res = 0x00;
    
    Test_SDCard_Send_Cmd(25, sector<<9, 0xFF, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
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
    return 0;
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


static int Send_8_Bytes(uint8_t data[])
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


static int Test_Get_CSD(void)
{
    uint32_t rev = 0;
    
    Set_DSPI_PUSHR(SDCard_Dev_Data_1.DSPI_dev, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    Test_SDCard_Send_Cmd(9, 0x00000000, 0xFF, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
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
    Rev_8_Bytes(SDCard_Dev_Data_1.CSD);
    Rev_8_Bytes(SDCard_Dev_Data_1.CSD + 8);
    Rev_8_Bytes(NULL);
    Rev_8_Bytes(NULL);
    Send_8_Clocks_withoout_CS();
    return 0;
}


int Get_SDCard_Size(uint32_t *size)
{
    uint32_t cnt = 0;
    
    cnt |= (uint32_t)(SDCard_Dev_Data_1.CSD[7])                 >> 6;
    cnt |= (uint32_t)(SDCard_Dev_Data_1.CSD[8])                 << 2;
    cnt |= (uint32_t)(SDCard_Dev_Data_1.CSD[9] & 0b00000011)    << 10;
    *size = cnt;
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
