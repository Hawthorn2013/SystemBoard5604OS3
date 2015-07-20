#include "includes.h"




int main(void) {
	volatile int i = 0;
	
	Init_ModesAndClock();
	Init_LED();
	Init_EMIOS_0();
	Init_Key(PCR_BSP_S1, EMIOS_0_UC_BSP_S1, IRQ_BSP_S1_S2, INTC_Handler_BSP_S1_S2);
	Enable_IRQ();
	
	/* Loop forever */
    for (;;) {											   /* Initialize the ticker, and other BSP related functions   */
        i++;
    }
}



