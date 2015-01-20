/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                     Freescale  MPC55xx Specific code
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : APP_CFG.H
* By      : Fabiano Kovalski
*********************************************************************************************************
*/

#ifndef _APP_CFG_H_
#define _APP_CFG_H_

/*
****************************************************************************************************
*                                          TASK PRIORITIES
****************************************************************************************************
*/
#define APP_TASK_START_PRIO     0

#define APP_TASK_1_PRIO         1
#define APP_TASK_2_PRIO         2
#define APP_TASK_3_PRIO         3
#define APP_TASK_4_PRIO         4
#define APP_TASK_5_PRIO         5
#define APP_TASK_6_PRIO         6

#define OS_TASK_TMR_PRIO        8


/*
****************************************************************************************************
*                                         TASK STACK SIZES
****************************************************************************************************
*/

#define APP_TASK_START_STK_SIZE      256

#define APP_TASK_1_STK_SIZE          256
#define APP_TASK_2_STK_SIZE          256
#define APP_TASK_3_STK_SIZE          256
#define APP_TASK_4_STK_SIZE          256
#define APP_TASK_5_STK_SIZE          256
#define APP_TASK_6_STK_SIZE          256


#endif  /* #ifndef _APP_CFG_H_ */

