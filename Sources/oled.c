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
    return 0;
}


void Task_OLED_Flush_Mem(void *p_arg)
{
    INT8U err1 = 0, err2 = 0;
    int i = 0;
    
    (void) p_arg;
    while (1)
    {
        OSMutexPend(OLED_Display_Memory.Mutex_Pages, 0, &err1);
        OSMutexPend(DSPI_1_Device_Data.Mut_DSPI_1, 0, &err2);
        DSPI_1_Device_Data.CB_TX_Complete = &Resume_Task_OLED_Flush_Mem;
        for (i = 0; i < OLED_PAGE_MAX; i++)
        {
            if (!OLED_Display_Memory.page[i].dirty_seg_cnt)
            {
                int j = 0;
                uint8_t cmd[] = {0x00, 0x00, 0x00};
                
                cmd[0] = 0xB0 + (uint8_t)i;
                cmd[1] = ((OLED_Display_Memory.page[i].dirty_seg_start & 0xF0) >> 4) | 0x10;
                cmd[2] = (OLED_Display_Memory.page[i].dirty_seg_start & 0x0F);
                OLED_PIN_DC = OLED_DC_COMMAND;
                OSTaskSuspend(OS_PRIO_SELF);
                DSPI_ASYNC_Send_Data(&DSPI_1_Device_Data, cmd, sizeof(cmd));
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
        OSMutexPost(DSPI_1_Device_Data.Mut_DSPI_1);
        OSMutexPost(OLED_Display_Memory.Mutex_Pages);
    }
}


void Resume_Task_OLED_Flush_Mem(void)
{
    OSTaskResume(TASK_PRIO_OLED_FLUSH_MEM);
}


