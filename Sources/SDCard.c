/*
 * SDCard.c
 *
 *  Created on: Jul 27, 2015
 *      Author: Jian
 */


#include        "includes.h"


static      uint8_t             Test_SD_Buff[512];


int Test_SDCard(void)
{
    Open_DSPI_Dev(&DSPI_1_Device_Data);
    Set_DSPI_CTAR(&DSPI_1_Device_Data, SDCARD_DSPI_CTAR_DBR, SDCARD_DSPI_CTAR_CPOL, SDCARD_DSPI_CTAR_CPHA, SDCARD_DSPI_CTAR_LSBFE, SDCARD_DSPI_CTAR_PCSSCK, SDCARD_DSPI_CTAR_PASC, SDCARD_DSPI_CTAR_PDT, SDCARD_DSPI_CTAR_PBR, SDCARD_DSPI_CTAR_CSSCK, SDCARD_DSPI_CTAR_ASC, SDCARD_DSPI_CTAR_DT, SDCARD_DSPI_CTAR_BR);
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, SDCARD_DSPI_PUSHR_PCS);
    while(1)
    {
        Reset_SDCard();
    }
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
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, SDCARD_DSPI_PUSHR_CONT, DSPI_PUSHR_PCS_NONE);
    DSPI_SYNC_Send_and_Receive_Data(&DSPI_1_Device_Data, send, NULL, 1);
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
