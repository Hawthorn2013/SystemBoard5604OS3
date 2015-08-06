#include "includes.h"



FATFS fatfs_1;
const TCHAR mmc[] = "0:";
FIL fil_1;
const TCHAR path[] = "IAMCP.txt";
const TCHAR test_line[] = "Hi! I am CP!";
UINT bw;
FRESULT fr1;

 int main(void) {
	volatile int i = 0;

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
    fr1 = f_open(&fil_1, path, FA_OPEN_ALWAYS);
    fr1 = f_close(&fil_1);
    fr1 = f_open(&fil_1, path, FA_WRITE);
    fr1 = f_write(&fil_1, test_line, sizeof(test_line)-1, &bw);
    fr1 = f_close(&fil_1);
    while(1) {}
        
    Enable_IRQ();
    Test_OLED_Init();
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



