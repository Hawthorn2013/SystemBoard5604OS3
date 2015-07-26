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
    Init_OLED_Pin();
    Delay_ms(100);
}


int Init_OLED_Pin(void)
{
    Init_GPIO(OLED_PIN_DC_PCR);
    Set_GPIO(OLED_PIN_DC_PCR, 1);
    Init_GPIO(OLED_PIN_RST_PCR);
    Set_GPIO(OLED_PIN_RST_PCR, 1);
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


void Test_OLED_Init(void)
{
    Init_OLED_Pin();
    DSPI_1.MCR.R = 0x803f0001;     /* Configure DSPI_0 as master */
    DSPI_1.CTAR[0].R = 0x3E0A7729;  //未使用 用于发送8bits 调整极性为1，相位为1，调整波特率为低速31.25kbit/s
    DSPI_1.CTAR[1].R = 0x38087726;  //TF 极性为0，相位为0，baud rate=625k/s
    DSPI_1.CTAR[2].R = 0x3E0A7724;  //L3G4200D 极性为1，相位为1，baud rate=1m/s
    DSPI_1.CTAR[3].R = 0x380A7720;  //OLED 极性为0，相位为0，baud rate=8m/s
    DSPI_1.MCR.B.HALT = 0x0;         /* Exit HALT mode: go from STOPPED to RUNNING state*/
    SIU.PCR[34].R = 0x0604; //PC2 SCK_1
    //SIU.PSMI[7].R = 0;    //SCK_1 PCR[34]
    SIU.PCR[35].R = 0x0503; //PC3 CS0_1
    //SIU.PSMI[9].R = 0;    //CS0_1 PCR[35]
    SIU.PCR[36].R = 0x0104; //PC4 SIN_1
    //SIU.PSMI[8].R = 0;    //SIN_1 PCR[36]
    SIU.PCR[62].R = 0x0604; //PD14 CS1_1
    SIU.PCR[63].R = 0x0604; //PD15 CS2_1
    SIU.PCR[67].R = 0x0A04; //PE3 SOUT_1
    SIU.PCR[74].R = 0x0A04; //PE10 CS3_1
    SIU.PCR[75].R = 0x0A04; //PE11 CS4_1
    DSPI_1.RSER.B.TCFRE = 0;    //关闭传输完成中断
    
    OLED_PIN_RST = 0;
    Delay_ms(1000);
    OLED_PIN_RST = 1;
    
    OLED_PIN_DC = 0;
    Test_DSPI_1_Send_Ex(0xae, 0x00, 2);
//    Test_DSPI_1_Send(0xae);//--turn off oled panel
//    Test_DSPI_1_Send(0x00);//---set low column address
    Test_DSPI_1_Send_Ex(0x10, 0x40, 2);
//    Test_DSPI_1_Send(0x10);//---set high column address
//    Test_DSPI_1_Send(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    Test_DSPI_1_Send_Ex(0x81, 0xcf, 2);
//    Test_DSPI_1_Send(0x81);//--set contrast control register
//    Test_DSPI_1_Send(0xcf); // Set SEG Output Current Brightness
    Test_DSPI_1_Send_Ex(0xa1, 0xc8, 2);
//    Test_DSPI_1_Send(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//    Test_DSPI_1_Send(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    Test_DSPI_1_Send_Ex(0xa6, 0xa8, 2);
//    Test_DSPI_1_Send(0xa6);//--set normal display
//    Test_DSPI_1_Send(0xa8);//--set multiplex ratio(1 to 64)
    Test_DSPI_1_Send_Ex(0x3f, 0xd3, 2);
//    Test_DSPI_1_Send(0x3f);//--1/64 duty
//    Test_DSPI_1_Send(0xd3);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    Test_DSPI_1_Send_Ex(0x00, 0xd5, 2);
//    Test_DSPI_1_Send(0x00);//-not offset
//    Test_DSPI_1_Send(0xd5);//--set display clock divide ratio/oscillator frequency
    Test_DSPI_1_Send_Ex(0x80, 0xd9, 2);
//    Test_DSPI_1_Send(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
//    Test_DSPI_1_Send(0xd9);//--set pre-charge period
    Test_DSPI_1_Send_Ex(0xf1, 0xda, 2);
//    Test_DSPI_1_Send(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    Test_DSPI_1_Send(0xda);//--set com pins hardware configuration
    Test_DSPI_1_Send_Ex(0x12, 0xdb, 2);
//    Test_DSPI_1_Send(0x12);
//    Test_DSPI_1_Send(0xdb);//--set vcomh
    Test_DSPI_1_Send_Ex(0x40, 0x20, 2);
//    Test_DSPI_1_Send(0x40);//Set VCOM Deselect Level
//    Test_DSPI_1_Send(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    Test_DSPI_1_Send_Ex(0x02, 0x8d, 2);
//    Test_DSPI_1_Send(0x02);//
//    Test_DSPI_1_Send(0x8d);//--set Charge Pump enable/disable
    Test_DSPI_1_Send_Ex(0x14, 0xa4, 2);
//    Test_DSPI_1_Send(0x14);//--set(0x10) disable
//    Test_DSPI_1_Send(0xa4);// Disable Entire Display On (0xa4/0xa5)
    Test_DSPI_1_Send_Ex(0xa6, 0xaf, 2);
//    Test_DSPI_1_Send(0xa6);// Disable Inverse Display On (0xa6/a7) 
//    Test_DSPI_1_Send(0xaf);//--turn on oled panel
    
    {
        int cnt = 0;
        while (1)
        {
            int x, y;
            for(y = 0; y < OLED_PAGE_MAX; y++)
            {
                OLED_PIN_DC = 0;
                Test_DSPI_1_Send_Ex((uint8_t)(0xb0 + y), 0x01, 2);
                Test_DSPI_1_Send_Ex(0x01, 0x00, 1);
//                Test_DSPI_1_Send((uint8_t)(0xb0 + y));
//                Test_DSPI_1_Send(0x01);
//                Test_DSPI_1_Send(0x10);             
                for(x = 0; x < OLED_SEG_MAX; x++)
                {
                    OLED_PIN_DC = 1;
                    if (0 == cnt)
                    {
                        if (y%2)
                        {
                            Test_DSPI_1_Send(0xFF);
                        }
                        else
                        {
                            Test_DSPI_1_Send(0x00);
                        }
                    }
                    else
                    {
                        if (y%2)
                        {
                            Test_DSPI_1_Send(0x00);
                        }
                        else
                        {
                            Test_DSPI_1_Send(0xFF);
                        }
                    }
                }
            }
            cnt++;
            cnt %= 2;
        }
    }
    while(1) {}
}


void Resume_Task_OLED_Flush_Mem(void)
{
    OSTaskResume(TASK_PRIO_OLED_FLUSH_MEM);
}


