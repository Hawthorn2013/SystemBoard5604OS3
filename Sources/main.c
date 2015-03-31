#include "ucos/includes.h"




int main(void) {
	volatile int i = 0;

    asm(" wrteei 0");
	Init_ModesAndClock();
	Init_LQBoard_LED();
	init_serial_port_1();
	init_LED();
	asm(" wrteei 0");
	Init_Pit();
    OSInit();
    SIU.PCR[1].R=0x0100; 
    //BD3=1;
    BD4=1;
    OSTaskCreate(  Test1Task,
                   (void *)0,
                   &Test1TaskStk[Test1_TASK_STK_SIZE-1],
                   TEST1_TASK_PRIO );
    INTC.CPR.B.PRI = 0;
    //asm(" wrteei 1");
//	Enable_IRQ();
    OSStart();	/* Start multitasking (i.e. give control to uC/OS-II)       */


	/* Loop forever */
    for (;;) {											   /* Initialize the ticker, and other BSP related functions   */
        i++;
    }
}



