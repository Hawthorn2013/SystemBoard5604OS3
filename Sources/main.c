#include "includes.h"




int main(void) {
  volatile int i = 0;

  


  /* Loop forever */
  for (;;) {											   /* Initialize the ticker, and other BSP related functions   */
	OSInit();
	OSTaskCreate(  Test1Task,
					(void *)0,
					&Test1TaskStk[Test1_TASK_STK_SIZE-1],
					TEST1_TASK_PRIO );
	OSStart();	/* Start multitasking (i.e. give control to uC/OS-II)       */
  }
}



