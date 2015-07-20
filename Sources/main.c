#include "includes.h"




int main(void) {
	volatile int i = 0;

    asm(" wrteei 0");
	Init_ModesAndClock();
	Init_LED();
	Turn_off_LED(PCR_BSP_LED1);
	Turn_off_LED(PCR_BSP_LED2);
	Turn_off_LED(PCR_BSP_LED3);
	Turn_off_LED(PCR_BSP_LED4);
	Turn_on_LED(PCR_BSP_LED1);
    Turn_on_LED(PCR_BSP_LED2);
    Turn_on_LED(PCR_BSP_LED3);
    Turn_on_LED(PCR_BSP_LED4);
    Toggle_LED(PCR_BSP_LED3);
    Toggle_LED(PCR_BSP_LED1);
    Toggle_LED(PCR_BSP_LED2);
    Toggle_LED(PCR_BSP_LED4);
    Toggle_LED(PCR_BSP_LED3);
    Toggle_LED(PCR_BSP_LED1);
    Toggle_LED(PCR_BSP_LED2);
    Toggle_LED(PCR_BSP_LED4);

	/* Loop forever */
    for (;;) {											   /* Initialize the ticker, and other BSP related functions   */
        i++;
    }
}



