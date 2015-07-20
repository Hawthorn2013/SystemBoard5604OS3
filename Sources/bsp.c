/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                 Freescale MPC55xx Board Support Package
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : BSP.C
* By      : Fabiano Kovalski
*********************************************************************************************************
*/

#include "includes.h"


        void    BSP_TmrTickISR(void);


/*$PAGE*/
/*
*********************************************************************************************************
*                                       TICK ISR
*
* Description: This function is the periodical time source of the uC/OS-II.
*
* Arguments  : none
*
* Note(s)    : BSP_TmrTickISR() MUST call OSTimeTick().
*********************************************************************************************************
*/

void  BSP_TmrTickISR (void)
{
    BD1 = ~BD1;
    OSTimeTick();
}


/*
 * Init pin to GPIO
 * Output high
 * Success return 0
 */
int Init_GPIO(uint16_t pcrNo)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    SIU.PCR[pcrNo].R = 0x0203;
    SIU.GPDO[pcrNo].R = 1;
    return 0;
}


/*
 * Set GPIO
 * Success return 0
 */
int Set_GPIO(uint16_t pcrNo, int value)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    if (value)
    {
        SIU.GPDO[pcrNo].R = 1;
    }
    else
    {
        SIU.GPDO[pcrNo].R = 0;
    }
    return 0;
}


/*
 * Get GPIO
 * Success return 0
 */
int Get_GPIO(uint16_t pcrNo, int *value)
{
    if (pcrNo >= sizeof(SIU.PCR))
    {
        return 1;
    }
    *value = SIU.GPDO[pcrNo].R;
    return 0;
}


/*
 * Init BSP LED
 * LED will be turned on
 */
int Init_LED(void)
{
    Init_GPIO(PCR_BSP_LED1);
    Set_GPIO(PCR_BSP_LED1, LED_TURN_ON_VALUE);
    Init_GPIO(PCR_BSP_LED2);
    Set_GPIO(PCR_BSP_LED2, LED_TURN_ON_VALUE);
    Init_GPIO(PCR_BSP_LED3);
    Set_GPIO(PCR_BSP_LED3, LED_TURN_ON_VALUE);
    Init_GPIO(PCR_BSP_LED4);
    Set_GPIO(PCR_BSP_LED4, LED_TURN_ON_VALUE);
    return 0;
}


/*
 * Turn on LED
 * Success return 0
 */
int Turn_on_LED(uint16_t pcrNo)
{
    return Set_GPIO(pcrNo, LED_TURN_ON_VALUE);
}


/*
 * Turn off LED
 * Success return 0
 */
int Turn_off_LED(uint16_t pcrNo)
{
    return Set_GPIO(pcrNo, LED_TURN_OFF_VALUE);
}



/*
 * Toggle LED
 * Success return 0
 */
int Toggle_LED(uint16_t pcrNo)
{
    int tmpVal;
    if (Get_GPIO(pcrNo, &tmpVal))
    {
        return 1;
    }
    if (Set_GPIO(pcrNo, !tmpVal))
    {
        return 2;
    }
    return 0;
}
