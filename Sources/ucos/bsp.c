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

#include <includes.h>


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
    OSTimeTick();
}
