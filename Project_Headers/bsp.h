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

/*
**************************************************************************************************************
*                                            LED SERVICES
**************************************************************************************************************
*/

#define     PCR_BSP_LED1                    (68)
#define     PCR_BSP_LED2                    (69)
#define     PCR_BSP_LED3                    (70)
#define     PCR_BSP_LED4                    (71)
#define     PIN_VASLUE_LED_TURN_ON          (0)
#define     PIN_VASLUE_LED_TURN_OFF         (1)
#if 1
#define     LED1                            (SIU.GPDO[PCR_BSP_LED1].B.PDO)
#define     LED2                            (SIU.GPDO[PCR_BSP_LED2].B.PDO)
#define     LED3                            (SIU.GPDO[PCR_BSP_LED3].B.PDO)
#define     LED4                            (SIU.GPDO[PCR_BSP_LED4].B.PDO)
#endif
/*
**************************************************************************************************************
*                                            KEY SERVICES
**************************************************************************************************************
*/

#define     EMIOS_MODE_SAIC                         (0x02)      /* Single Action Input Capture */
#define     EMIOS_BSL_COUNTER_BUS_A                 (0x00)
#define     EMIOS_BSL_COUNTER_BUS_BCDE              (0x01)
#define     EMIOS_BSL_INTERNAL_BUS                  (0x02)
#define     EMIOS_EDSEL_BOTH_TRIGGERING             (0x01)
#define     EMIOS_EDSEL_SINGLE_TRIGGERING           (0x00)
#define     EMIOS_EDPOL_TRIGGER_ON_RISING_EDGE      (0x01)
#define     EMIOS_EDPOL_TRIGGER_ON_FALLING_EDGE     (0x00)
#define     PCR_BSP_S1                              (64)
#define     PCR_BSP_S2                              (65)
#define     PCR_BSP_S3                              (66)
#define     PCR_BSP_S4                              (67)
#define     EMIOS_0_UC_BSP_S1                       (16)
#define     EMIOS_0_UC_BSP_S2                       (17)
#define     EMIOS_0_UC_BSP_S3                       (18)
#define     EMIOS_0_UC_BSP_S4                       (19)
#define     IRQ_BSP_S1_S2                           (149)
#define     IRQ_BSP_S3_S4                           (150)
#define     INTC_PRIORITY_BSP_S1_S4                 (3)
#define     PIN_VALUE_KEY_UP                        (1)
#define     PIN_VALUE_KEY_DOWN                      (0)

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
extern int   Init_GPIO(uint16_t pcrNo);
extern int   Set_GPIO(uint16_t pcrNo, int value);
extern int   Get_GPIO(uint16_t pcrNo, int *value);
extern int   Init_LED(void);
extern int   Turn_on_LED(uint16_t pcrNo);
extern int   Turn_off_LED(uint16_t pcrNo);
extern int   Toggle_LED(uint16_t pcrNo);

/*
**************************************************************************************************************
*                                            KEY SERVICES
**************************************************************************************************************
*/

extern void  Init_EMIOS_0(void);
extern int   Init_Key(uint16_t pcr, uint16_t uc, uint16_t irq, INTCInterruptFn handler);
extern void  INTC_Handler_BSP_S1_S2(void);

/*
*********************************************************************************************************
*                                            MISCELLANEOUS
*********************************************************************************************************
*/

#ifndef CPU_CONST_FREQ
  #error   "BSP.H, Missing CPU_CONST_FREQ: Enable (1) or Disable (0) automatic CPU frequency check at CPU_CLK_FREQ()"
#endif

