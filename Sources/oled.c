/*
 * oled.c
 *
 *  Created on: Jul 23, 2015
 *      Author: Jian
 */


#include "includes.h"


OS_STK TaskStk_OLED_Flush_Mem[TASK_STK_SIZE_OLED_FLUSH_MEM-1];



static struct
{
    OS_EVENT *Mutex_Pages;
    struct
    {
        uint8_t dirty_seg_start;
        uint8_t dirty_seg_cnt;
        uint8_t data[OLED_SEG_MAX];
    } page[OLED_PAGE_MAX];
} OLED_Display_Memory;


OS_EVENT Sem_OLED_Display_Memory, Sem_OLED_Flush_Mem;


int Init_OLED(void)
{
    INT8U err1 = 0;
    
    OLED_Display_Memory.Mutex_Pages = OSMutexCreate(TASK_PRIO_MUTEX_OLED_DIS_MEM, &err1);
    Set_DSPI_CTAR(&DSPI_1_Device_Data, OLED_DSPI_CTAR_DBR, OLED_DSPI_CTAR_CPOL, OLED_DSPI_CTAR_CPHA, OLED_DSPI_CTAR_LSBFE, OLED_DSPI_CTAR_PCSSCK, OLED_DSPI_CTAR_PASC, OLED_DSPI_CTAR_PDT, OLED_DSPI_CTAR_PBR, OLED_DSPI_CTAR_CSSCK, OLED_DSPI_CTAR_ASC, OLED_DSPI_CTAR_DT, OLED_DSPI_CTAR_BR);
    Set_DSPI_PUSHR(&DSPI_1_Device_Data, OLED_DSPI_PUSHR_CONT, OLED_DSPI_PUSHR_PCS);
    Init_GPIO(OLED_PIN_DC_PCR);
    Set_GPIO(OLED_PIN_DC_PCR, 1);
    Init_GPIO(OLED_PIN_RST_PCR);
    Set_GPIO(OLED_PIN_RST_PCR, 1);
    Delay_ms(100);
}


int Fill_OLED_PAGE(int page, int seg_start, int seg_cnt, int pixel_appearance)
{
    int i = 0;
    uint8_t fill;
    INT8U err1;
    
    if (page < 0 || page >= OLED_PAGE_MAX)
    {
        return 1;
    }
    if (seg_start < 0 || seg_start >= OLED_SEG_MAX)
    {
        return 2;
    }
    if (seg_cnt < 0 || seg_cnt + seg_start >= OLED_SEG_MAX)
    {
        return 3;
    }
    if (!OSMutexAccept(OLED_Display_Memory.Mutex_Pages, &err1))
    {
        return 4;
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
        OLED_Display_Memory.page[page].data[i] = fill;
    }
    OSMutexPost(OLED_Display_Memory.Mutex_Pages);
    OSTaskResume(TASK_PRIO_OLED_FLUSH_MEM);    
    return 0;
}


void Task_OLED_Flush_Mem(void *p_arg)
{
    INT8U err1 = 0, err2 = 0, err3 = 0;
    int i = 0;
    
    (void) p_arg;
    {
        OLED_PIN_RST = 0;
        OSTimeDly(100);
        OLED_PIN_RST = 1;
        OSTimeDly(5);
    }
    {
        uint8_t cmds[] = {0xae, 0x00, 0x10, 0x40, 0x81, 0xcf, 0xa1, 0xc8, 0xa6, 0xa8, 0x3f, 0xd3, 0x00, 0xd5, 0x80, 0xd9, 0xf1, 0xda, 0x12, 0xdb, 0x40, 0x20, 0x02, 0x8d, 0x14, 0xa4, 0xa6, 0xaf, };
        int i = 0;
        INT8U err1 = 0;
        uint32_t test1234[] = {0x12345678, 0xECECECEC};
        
        Dummy((uint8_t *)test1234);
        OSMutexPend(DSPI_1_Device_Data.Mut_DSPI_1, 0, &err3);
        DSPI_1_Device_Data.CB_TX_Complete = &Resume_Task_OLED_Flush_Mem;
        Enable_INTC_DSPI_SR_EOQF(DSPI_1_Device_Data.dspi);
        OLED_PIN_DC = OLED_DC_COMMAND;
        for (i = 0; i < sizeof(cmds); i += DSPI_ASYNC_SEND_DATA_MAX_LENGTH)
        {
            int remain = 0;
            
            remain = sizeof(cmds) - i;
            DSPI_ASYNC_Send_Data(&DSPI_1_Device_Data, cmds, (remain < DSPI_ASYNC_SEND_DATA_MAX_LENGTH ? remain : DSPI_ASYNC_SEND_DATA_MAX_LENGTH));
            OSTaskSuspend(OS_PRIO_SELF);
        }
        Disable_INTC_DSPI_SR_EOQF(DSPI_1_Device_Data.dspi);
        DSPI_1_Device_Data.CB_TX_Complete = NULL;
        OSMutexPost(DSPI_1_Device_Data.Mut_DSPI_1);
    }
    while (1)
    {
        LED2 = ~LED2;
        OSTaskSuspend(OS_PRIO_SELF);
        OSMutexPend(OLED_Display_Memory.Mutex_Pages, 0, &err1);
        OSMutexPend(DSPI_1_Device_Data.Mut_DSPI_1, 0, &err2);
        DSPI_1_Device_Data.CB_TX_Complete = &Resume_Task_OLED_Flush_Mem;
        Enable_INTC_DSPI_SR_EOQF(DSPI_1_Device_Data.dspi);
        for (i = 0; i < OLED_PAGE_MAX; i++)
        {
            if (!OLED_Display_Memory.page[i].dirty_seg_cnt)
            {
                int j = 0;
                uint8_t cmd[] = {0x00, 0x00, 0x00};

                OLED_PIN_DC = OLED_DC_COMMAND;
                cmd[0] = 0xB0 + (uint8_t)i;
                cmd[1] = ((OLED_Display_Memory.page[i].dirty_seg_start & 0xF0) >> 4) | 0x10;
                cmd[2] = (OLED_Display_Memory.page[i].dirty_seg_start & 0x0F);
                OLED_PIN_DC = OLED_DC_COMMAND;
                DSPI_ASYNC_Send_Data(&DSPI_1_Device_Data, cmd, sizeof(cmd));
                OSTaskSuspend(OS_PRIO_SELF);
                OLED_PIN_DC = OLED_DC_DATA;
                for (j = 0; j < OLED_Display_Memory.page[i].dirty_seg_cnt; j += DSPI_ASYNC_SEND_DATA_MAX_LENGTH)
                {
                    int remain = 0;
                    
                    remain = OLED_Display_Memory.page[i].dirty_seg_cnt - i * DSPI_ASYNC_SEND_DATA_MAX_LENGTH;
                    DSPI_ASYNC_Send_Data(&DSPI_1_Device_Data, OLED_Display_Memory.page[i].data, (remain < DSPI_ASYNC_SEND_DATA_MAX_LENGTH ? remain : DSPI_ASYNC_SEND_DATA_MAX_LENGTH));
                    OSTaskSuspend(OS_PRIO_SELF);
                }
            }
        }
        Disable_INTC_DSPI_SR_EOQF(DSPI_1_Device_Data.dspi);
        DSPI_1_Device_Data.CB_TX_Complete = NULL;
        OSMutexPost(DSPI_1_Device_Data.Mut_DSPI_1);
        OSMutexPost(OLED_Display_Memory.Mutex_Pages);
    }
}


void Resume_Task_OLED_Flush_Mem(void)
{
    OSTaskResume(TASK_PRIO_OLED_FLUSH_MEM);
}


