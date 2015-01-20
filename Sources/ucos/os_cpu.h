/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                     Freescale MPC55xx Specific code
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : OS_CPU.H
* By      : Fabiano Kovalski
*
* LICENSING TERMS:
* ---------------
*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
* If you plan on using  uC/OS-II  in a commercial product you need to contact Micriµm to properly  license
* its use in your product.  We provide ALL the source code for your convenience and to help you experience
* uC/OS-II.   The fact that the  source  is provided does  NOT  mean that you can use it without  paying a
* licensing fee.
*********************************************************************************************************
*/

#ifndef __OS_CPU_H__                                                    /* test for multiple inclusion                              */
#define __OS_CPU_H__

/*
*********************************************************************************************************
*                                             INCLUDES
*********************************************************************************************************
*/

#include  <cpu.h>

#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif


/*
*********************************************************************************************************
*                                           DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_BOOLEAN    BOOLEAN;
typedef  CPU_INT08U     INT8U;                                          /* Unsigned  8 bit quantity                                 */
typedef  CPU_INT08S     INT8S;                                          /* Signed    8 bit quantity                                 */
typedef  CPU_INT16U     INT16U;                                         /* Unsigned 16 bit quantity                                 */
typedef  CPU_INT16S     INT16S;                                         /* Signed   16 bit quantity                                 */
typedef  CPU_INT32U     INT32U;                                         /* Unsigned 32 bit quantity                                 */
typedef  CPU_INT32S     INT32S;                                         /* Signed   32 bit quantity                                 */
typedef  CPU_FP32       FP32;                                           /* Single precision floating point                          */
typedef  CPU_FP64       FP64;                                           /* Double precision floating point                          */

typedef  CPU_STK        OS_STK;                                         /* Define size of CPU stack entry                           */
typedef  CPU_SR         OS_CPU_SR;                                      /* Define size of CPU status register                       */


/*
*********************************************************************************************************
*                                           DEFINES
*********************************************************************************************************
*/

#define  OS_STK_RSVD_SIZE    10                                         /* EBI Buffer above the stack                               */

#define  OS_STK_GROWTH        1                                         /* Stack grows from HIGH to LOW memory on PPC               */


/*
*********************************************************************************************************
*                                           FLOATING POINT
*
* Note: also enable or disable "OS_SAVE_CONTEXT_WITH_FPRS .equ 1" in os_cpu_a.h
*********************************************************************************************************
*/

#define  OS_SAVE_CONTEXT_WITH_FPRS


/*
*********************************************************************************************************
*                                              OS Task Swicth
*********************************************************************************************************
*/

#if   defined __GNUC__
#define  OS_TASK_SW()       asm __volatile__ (" se_sc ");
#elif defined __MWERKS__
#define  OS_TASK_SW()       asm (" se_sc ");
#else
#error Unknown Compiler Assembler Syntax
#endif


/*
*********************************************************************************************************
*                                           Critical Method MACROS
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD     CPU_CFG_CRITICAL_METHOD

#define  OS_ENTER_CRITICAL()   {CPU_CRITICAL_ENTER();}
#define  OS_EXIT_CRITICAL()    {CPU_CRITICAL_EXIT();}


/*
*********************************************************************************************************
*                                         Function Prototypes
*********************************************************************************************************
*/

void  OSCtxSw(void);
void  OSIntCtxSw(void);
void  OSStartHighRdy(void);

void  OSTickISR(void);
void  OSExtIntISR(void);


#endif  /* __OS_CPU_H__ */
