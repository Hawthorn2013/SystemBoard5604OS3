/*
 * SDCard.c
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */


#include        "includes.h"


static      uint8_t             Test_SD_Buff[4][SDCARD_SECTOR_SIZE];


int Test_SDCard(void)
{
    Open_DSPI_Dev(&DSPI_1_Device_Data);
    Set_DSPI_CTAR(&DSPI_1_Device_Data, SDCARD_DSPI_CTAR_DBR, SDCARD_DSPI_CTAR_CPOL, SDCARD_DSPI_CTAR_CPHA, SDCARD_DSPI_CTAR_LSBFE, SDCARD_DSPI_CTAR_PCSSCK, SDCARD_DSPI_CTAR_PASC, SDCARD_DSPI_CTAR_PDT, SDCARD_DSPI_CTAR_PBR, SDCARD_DSPI_CTAR_CSSCK, SDCARD_DSPI_CTAR_ASC, SDCARD_DSPI_CTAR_DT, SDCARD_DSPI_CTAR_BR);
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    Reset_SDCard();
    Test_SDCard_Read_Mult_Block(0, (uint8_t (*)[SDCARD_SECTOR_SIZE])(Test_SD_Buff[0]), 2);
    Test_SDCard_Write_Block(2, Test_SD_Buff[0]);
    Test_SDCard_Write_Block(3, Test_SD_Buff[1]);
    Test_SDCard_Read_Block(2, Test_SD_Buff[2]);
    Test_SDCard_Read_Block(3, Test_SD_Buff[3]);
}


int Test_SDCard_Send_Cmd(int cmd, uint32_t var, uint32_t *rev)
{
    uint32_t sector = 0x00000000;
    uint8_t send[8];
    
    send[0] = 0xFF;
    send[1] = (uint8_t)(cmd | 0x40);
    *(uint32_t *)&(send[2]) = var;
    send[6] = 0x95;
    send[7] = 0xFF;
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send + 1, NULL, 6);
    do
    {
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, send + 7, 1);
    }
    while (0xFF == send[7]);
    *rev = send[7];
}


int Reset_SDCard(void)
{
    uint32_t rev = 0x00000000;
    
    {
        uint8_t send[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, };
        Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, DSPI_PUSHR_PCS_NONE);
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, NULL, 8);
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, NULL, 2);
    }
    do
    {
        Test_SDCard_Send_Cmd(0, 0, &rev);
    }
    while (0x01 != rev);
    do
    {
        Test_SDCard_Send_Cmd(1, 0, &rev);
    }
    while (0x00 != rev);
    Test_SDCard_Send_Cmd(59, 0, &rev);
    Test_SDCard_Send_Cmd(16, 512, &rev);
    return 0;
}


int Test_SDCard_Read_Block(uint32_t sector, uint8_t buffer[])
{
    uint32_t rev = 0x00000000;
    int i = 0;
    uint8_t send[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    
    Test_SDCard_Send_Cmd(17, sector<<9, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
    }
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    while(1)
    {
        int i = 0, ok = 0;
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, buffer, 8);
        for (i = 0; i < 8; i++)
        {
            if (0xFE == buffer[i])
            {
                int j = 0;
                i++;
                for (j = 0; j < 8 - i; j++)
                {
                    buffer[j] = buffer[j + i];
                }
                DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, buffer + j, 8 - j);
                ok = 1;
                break;
            }
        }
        if (ok)
        {
            break;
        }
    }
    for (i = 1; i < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; i++)
    {
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, buffer + DSPI_ASYNC_SEND_DATA_MAX_LENGTH * i, DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
    }
    DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, NULL, 2);
    return 0;
}


int Test_SDCard_Read_Mult_Block(uint32_t sector, uint8_t buffer[][SDCARD_SECTOR_SIZE], int block_cnt)
{
    uint32_t rev = 0x00000000;
    int i = 0, j = 0;
    uint8_t send[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };
    
    Test_SDCard_Send_Cmd(18, sector<<9, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
    }
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    
    for (i = 0; i < block_cnt; i++)
    {
        while(1)
        {
            uint8_t res = 0x00;
            DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, &res, 1);
            if (0xFE == res)
            {
                break;
            }
        }
        for (j = 0; j < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; j++)
        {
            
            DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, &buffer[i][DSPI_ASYNC_SEND_DATA_MAX_LENGTH * j], DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
        }
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, NULL, 2);
    }
    Test_SDCard_Send_Cmd(12, 0, &rev);
    return 0;
}


int Test_SDCard_Write_Block(uint32_t sector, uint8_t buffer[])
{
    uint32_t rev = 0x00000000;
    int i = 0;
    uint8_t send[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, };
    uint8_t res = 0x00;
    
    Test_SDCard_Send_Cmd(24, sector<<9, &rev);
    if (0x00 != rev)
    {
        return (int)rev;
    }
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send + 5, NULL, 4);
    for (i = 0; i < SDCARD_SECTOR_SIZE / DSPI_ASYNC_SEND_DATA_MAX_LENGTH; i++)
    {
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, &buffer[DSPI_ASYNC_SEND_DATA_MAX_LENGTH * i], NULL, DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
    }
    DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, NULL, 2);
    DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, &res, 1);
    if ((res & 0b00011111) != 0b00000101)
    {
        return SDCARD_ERR_WRITE_BLOCK_FAILED;
    }
    while(1)
    {
        uint8_t res = 0x00;
        DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, &res, 1);
        if (0xFF == res)
        {
            break;
        }
    }
    return 0;
}















