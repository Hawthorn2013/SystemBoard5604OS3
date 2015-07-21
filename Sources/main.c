#include "includes.h"




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
    Init_OSTickISR();
    OSInit();
    OSTaskCreate(  Test1Task,
                   (void *)0,
                   &Test1TaskStk[Test1_TASK_STK_SIZE-1],
                   TEST1_TASK_PRIO );
    OSTaskCreate(  Test2Task,
                   (void *)0,
                   &Test2TaskStk[Test2_TASK_STK_SIZE-1],
                   TEST2_TASK_PRIO );
    OSTaskCreate(  Test3Task,
                       (void *)0,
                       &Test3TaskStk[Test3_TASK_STK_SIZE-1],
                       TEST3_TASK_PRIO );
    OSStart();	/* Start multitasking (i.e. give control to uC/OS-II)       */


	/* Loop forever */
    for (;;) {											   /* Initialize the ticker, and other BSP related functions   */
        i++;
    }
}



