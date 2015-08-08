/*
 * oled.c
 *
 *  Created on: Jul 23, 2015
 *      Author: Jian
 */


#include "includes.h"


OS_STK TaskStk_OLED_Flush_Mem[TASK_STK_SIZE_OLED_FLUSH_MEM-1];


static uint8_t OLED_Display_Memory_2[OLED_PAGE_MAX][OLED_SEG_MAX];

typedef struct
{
    struct DSPI_Device_Data *DSPI_dev;
} OLED_Dev_Data;

static OLED_Dev_Data OLED_Dev_Data_1;


OLED_RES Init_OLED(void)
{
    OLED_Dev_Data_1.DSPI_dev = &DSPI_1_Device_Data;
    if (DSPI_RES_OK != Open_DSPI_Dev(OLED_Dev_Data_1.DSPI_dev))
     {
         return OLED_RES_BUS_BUSY;
     }
    Init_OLED_Pin();
    Set_DSPI_CTAR(OLED_Dev_Data_1.DSPI_dev, OLED_DSPI_CTAR_DBR, OLED_DSPI_CTAR_CPOL, OLED_DSPI_CTAR_CPHA, OLED_DSPI_CTAR_LSBFE, OLED_DSPI_CTAR_PCSSCK, OLED_DSPI_CTAR_PASC, OLED_DSPI_CTAR_PDT, OLED_DSPI_CTAR_PBR, OLED_DSPI_CTAR_CSSCK, OLED_DSPI_CTAR_ASC, OLED_DSPI_CTAR_DT, OLED_DSPI_CTAR_BR);
    Set_DSPI_PUSHR(OLED_Dev_Data_1.DSPI_dev, OLED_DSPI_PUSHR_CONT, OLED_DSPI_PUSHR_PCS);
    {
        OLED_PIN_RST = 0;
        Delay_ms(1000);
        OLED_PIN_RST = 1;
        Delay_ms(50);
    }
    {
        uint8_t cmds[] = {0xae, 0x00, 0x10, 0x40, 0x81, 0xcf, 0xa1, 0xc8, 0xa6, 0xa8, 0x3f, 0xd3, 0x00, 0xd5, 0x80, 0xd9, 0xf1, 0xda, 0x12, 0xdb, 0x40, 0x20, 0x02, 0x8d, 0x14, 0xa4, 0xa6, 0xaf, };
        int i = 0;
        INT8U err1 = 0;
        
        OLED_PIN_DC = OLED_DC_COMMAND;
//        while (1)
//        {
//            DSPI_SYNC_Send_and_Receive_Data(OLED_Dev_Data_1.DSPI_dev, cmds, NULL, 8);
//        }
        for (i = 0; i < sizeof(cmds); i += DSPI_ASYNC_SEND_DATA_MAX_LENGTH)
        {
            int remain = 0;
            
            remain = sizeof(cmds) - i;
            DSPI_SYNC_Send_and_Receive_Data(OLED_Dev_Data_1.DSPI_dev, cmds + i, NULL, (remain < DSPI_ASYNC_SEND_DATA_MAX_LENGTH ? remain : DSPI_ASYNC_SEND_DATA_MAX_LENGTH));
        }
    }
    Delay_ms(100);
    Close_DSPI(OLED_Dev_Data_1.DSPI_dev);
    return OLED_RES_OK;
}


int Init_OLED_Pin(void)
{
    Init_GPIO(OLED_PIN_DC_PCR);
    Set_GPIO(OLED_PIN_DC_PCR, 1);
    Init_GPIO(OLED_PIN_RST_PCR);
    Set_GPIO(OLED_PIN_RST_PCR, 1);
}


OLED_RES Fill_OLED_PAGE(int page, int seg_start, int seg_cnt, int pixel_appearance)
{
    int i = 0;
    uint8_t fill;
    
    if (page < 0 || page >= OLED_PAGE_MAX)
    {
        return OLED_RES_ERR_PAR;
    }
    if (seg_start < 0 || seg_start >= OLED_SEG_MAX)
    {
        return OLED_RES_ERR_PAR;
    }
    if (seg_cnt < 0 || seg_cnt + seg_start > OLED_SEG_MAX)
    {
        return OLED_RES_ERR_PAR;
    }
    if (OLED_PIXEL_APPEARANCE_INVISIBLE == pixel_appearance)
    {
        fill = 0x00;
    }
    else
    {
        fill = 0xff;
    }
    for (i = seg_start; i < seg_start + seg_cnt; i++)
    {
        OLED_Display_Memory_2[page][i] = fill;
    }
    return OLED_RES_OK;
}


OLED_RES Flush_OLED_Mem(void)
 {
     int i;
     uint8_t cmd[] = {0x00, 0x00, 0x00};
     
     if (DSPI_RES_OK != Open_DSPI_Dev(OLED_Dev_Data_1.DSPI_dev))
     {
         return OLED_RES_BUS_BUSY;
     }
     Set_DSPI_CTAR(OLED_Dev_Data_1.DSPI_dev, OLED_DSPI_CTAR_DBR, OLED_DSPI_CTAR_CPOL, OLED_DSPI_CTAR_CPHA, OLED_DSPI_CTAR_LSBFE, OLED_DSPI_CTAR_PCSSCK, OLED_DSPI_CTAR_PASC, OLED_DSPI_CTAR_PDT, OLED_DSPI_CTAR_PBR, OLED_DSPI_CTAR_CSSCK, OLED_DSPI_CTAR_ASC, OLED_DSPI_CTAR_DT, OLED_DSPI_CTAR_BR);
     Set_DSPI_PUSHR(OLED_Dev_Data_1.DSPI_dev, OLED_DSPI_PUSHR_CONT, OLED_DSPI_PUSHR_PCS);
     OLED_PIN_DC = OLED_DC_DATA;
     for (i = 0; i < sizeof(OLED_Display_Memory_2); i += DSPI_ASYNC_SEND_DATA_MAX_LENGTH)
     {
         DSPI_SYNC_Send_and_Receive_Data(OLED_Dev_Data_1.DSPI_dev, ((uint8_t *)OLED_Display_Memory_2 + i * DSPI_ASYNC_SEND_DATA_MAX_LENGTH), NULL, DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
     }
     for (i = 0; i < OLED_PAGE_MAX; i++)
     {
         int j = 0;
         uint8_t cmd[] = {0x00, 0x00, 0x00};

         OLED_PIN_DC = OLED_DC_COMMAND;
         cmd[0] = 0xB0 + (uint8_t)i;
         cmd[1] = (((uint8_t)0 & 0xF0) >> 4) | 0x10;
         cmd[2] = ( (uint8_t)0 & 0x0F);
         OLED_PIN_DC = OLED_DC_COMMAND;
         DSPI_SYNC_Send_and_Receive_Data(OLED_Dev_Data_1.DSPI_dev, cmd, NULL, sizeof(cmd));
         OLED_PIN_DC = OLED_DC_DATA;
         for (j = 0; j < OLED_SEG_MAX; j += DSPI_ASYNC_SEND_DATA_MAX_LENGTH)
         {
             DSPI_SYNC_Send_and_Receive_Data(OLED_Dev_Data_1.DSPI_dev, OLED_Display_Memory_2[i] + j, NULL, DSPI_ASYNC_SEND_DATA_MAX_LENGTH);
         }
     }
     Close_DSPI(OLED_Dev_Data_1.DSPI_dev);
     return OLED_RES_OK;
 }
