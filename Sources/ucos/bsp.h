/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                 Freescale MPC55xx Board Support Package
*
*                                 (c) Copyright 2007; Micrium; Weston, FL
*                                           All Rights Reserved
*
* File    : BSP.H
* By      : Fabiano Kovalski
*********************************************************************************************************
*/


/*
**************************************************************************************************************
*                                            CPU CLOCK FREQUENCY
**************************************************************************************************************
*/

#define     CPU_CONST_FREQ                     1                        /* Enable automatic CPU operating frequency check           */

#if CPU_CONST_FREQ > 0
  #define   CPU_CLK_FREQ()              BSP_CPU_ClkFreq()
#else
  #define   CPU_CLK_FREQ()            (132000000)                       /* CPU operating frequency                                  */
#endif

/*
**************************************************************************************************************
*                                            INTERRUPT CONTROLLER
**************************************************************************************************************
*/

#define     BSP_INTC_ERR_NONE                  0                        /* BSP_INTC_IntReg is successful                            */
#define     BSP_INTC_ERR_INVALID_IRQ           1                        /* BSP_INTC_IntReg received an invalid IRQ #                */

#define     BSP_USART_INT_RDRF_MASK         0x20
#define     BSP_USART_INT_TDRE_MASK         0x80

#define     BSP_USART_INT_RDRF_STATUS 0x20000000
#define     BSP_USART_INT_TDRE_STATUS 0x80000000


/*$PAGE*/
/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         BSP_Init(void);

CPU_INT32U   BSP_INTC_IntReg(CPU_FNCT_VOID handler, CPU_INT32U irq, CPU_INT32U int_level);

void         BSP_GPIO_SetFnct(CPU_INT16U pin, CPU_INT16U function);
CPU_BOOLEAN  BSP_GPIO_PinGet(CPU_INT16U pin);
void         BSP_GPIO_PinSet(CPU_INT16U pin);
void         BSP_GPIO_PinClr(CPU_INT16U pin);
void         BSP_GPIO_PinTgl(CPU_INT16U pin);

CPU_INT32U   BSP_CPU_ClkFreq(void);

void         BSP_USART_Init(CPU_INT08U com, CPU_INT32U baud_rate);
void         BSP_USART_ByteWr(CPU_INT08U com, CPU_INT08U b);
CPU_INT08U   BSP_USART_ByteRd(CPU_INT08U com);
void         BSP_USART_StrWr(CPU_INT08U com, CPU_INT08U *s);
void         BSP_USART_IntEn(CPU_INT08U com, CPU_INT32U mask);
void         BSP_USART_IntDis(CPU_INT08U com, CPU_INT32U mask);


/*
*********************************************************************************************************
*                                             LED SERVICES
*********************************************************************************************************
*/

void         LED_On(CPU_INT08U led);
void         LED_Off(CPU_INT08U led);
void         LED_Toggle(CPU_INT08U led);

/*
*********************************************************************************************************
*                                            MISCELLANEOUS
*********************************************************************************************************
*/

#ifndef CPU_CONST_FREQ
  #error   "BSP.H, Missing CPU_CONST_FREQ: Enable (1) or Disable (0) automatic CPU frequency check at CPU_CLK_FREQ()"
#endif

