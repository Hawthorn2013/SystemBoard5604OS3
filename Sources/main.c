#include "includes.h"



FATFS fatfs_1;
const TCHAR mmc[] = L"0:";
FIL fil_1, fil_2;
const TCHAR path2[] = L"IAM陈鹏hahahahaha.txt";
const char test_line[] = "Hi! I am 陈鹏!";
UINT bw;
FRESULT fr1;

 int main(void) {
	volatile int i = 0;
	uint8_t Font_Data[FONT_HEIGHT * FONT_WIDTH / 8];
	
	Init_ModesAndClock();
	Init_LED();
	Init_EMIOS_0();
    Init_Key(PCR_BSP_S1, EMIOS_0_UC_BSP_S1, IRQ_BSP_S1_S2, INTC_Handler_BSP_S1_S2);
    Init_Key(PCR_BSP_S2, EMIOS_0_UC_BSP_S2, IRQ_BSP_S1_S2, INTC_Handler_BSP_S1_S2);
    Init_Key(PCR_BSP_S3, EMIOS_0_UC_BSP_S3, IRQ_BSP_S3_S4, INTC_Handler_BSP_S3_S4);
    Init_Key(PCR_BSP_S4, EMIOS_0_UC_BSP_S4, IRQ_BSP_S3_S4, INTC_Handler_BSP_S3_S4);
    Init_UART_0_Ex();
    Init_DSPI_1();
        
    fr1 = f_mount(&fatfs_1, mmc, 1);
    Load_Font_File();
    
    Init_OLED();
    Fill_OLED_PAGE(0, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_VISIBLE);
    Fill_OLED_PAGE(1, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_INVISIBLE);
    Fill_OLED_PAGE(2, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_VISIBLE);
    Fill_OLED_PAGE(3, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_INVISIBLE);
    Fill_OLED_PAGE(4, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_VISIBLE);
    Fill_OLED_PAGE(5, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_INVISIBLE);
    Fill_OLED_PAGE(6, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_VISIBLE);
    Fill_OLED_PAGE(7, 0, OLED_SEG_MAX, OLED_PIXEL_APPEARANCE_INVISIBLE);
    Flush_OLED_Mem();
    
    Get_Font_16x16(L'蛤', Font_Data);
    for (i = 0; i < 16; i++)
    {
        OLED_Display_Memory_2[0][i] = Font_Data[i * 2 + 1];
        OLED_Display_Memory_2[1][i] = Font_Data[i * 2];
    }
    Flush_OLED_Mem();
        
    Enable_IRQ();
    OSInit();
    Init_OSTickISR();
    Init_OLED();
    OSTaskCreate(  Test1Task,
                   (void *)0,
                   &Test1TaskStk[Test1_TASK_STK_SIZE-1],
                   TEST1_TASK_PRIO );
//    OSTaskCreate(  Test2Task,
//                   (void *)0,
//                   &Test2TaskStk[Test2_TASK_STK_SIZE-1],
//                   TEST2_TASK_PRIO );
//    OSTaskCreate(  Test4Task,
//                   (void *)0,
//                   &Test4TaskStk[Test4_TASK_STK_SIZE-1],
//                   TEST4_TASK_PRIO );
//    OSTaskCreate(  Task_OLED_Flush_Mem,
//                   (void *)0,
//                   &TaskStk_OLED_Flush_Mem[TASK_STK_SIZE_OLED_FLUSH_MEM-1],
//                   TASK_PRIO_OLED_FLUSH_MEM );
    OSStart();	/* Start multitasking (i.e. give control to uC/OS-II)       */


	/* Loop forever */
    for (;;) {											   /* Initialize the ticker, and other BSP related functions   */
        i++;
    }
}



