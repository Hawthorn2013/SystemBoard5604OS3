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

/*
**************************************************************************************************************
*                                            UART SERVICES
**************************************************************************************************************
*/

#define LINFLEX_DIV_M_BAUD_RATE_2400                (416)
#define LINFLEX_DIV_F_BAUD_RATE_2400                (11)
#define LINFLEX_DIV_M_BAUD_RATE_9600                (104)
#define LINFLEX_DIV_F_BAUD_RATE_9600                (3)
#define LINFLEX_DIV_M_BAUD_RATE_10417               (96)
#define LINFLEX_DIV_F_BAUD_RATE_10417               (0)
#define LINFLEX_DIV_M_BAUD_RATE_19200               (52)
#define LINFLEX_DIV_F_BAUD_RATE_19200               (1)
#define LINFLEX_DIV_M_BAUD_RATE_57600               (17)
#define LINFLEX_DIV_F_BAUD_RATE_57600               (6)
#define LINFLEX_DIV_M_BAUD_RATE_115200              (8)
#define LINFLEX_DIV_F_BAUD_RATE_115200              (11)
#define LINFLEX_DIV_M_BAUD_RATE_230400              (4)
#define LINFLEX_DIV_F_BAUD_RATE_230400              (5)
#define LINFLEX_DIV_M_BAUD_RATE_460800              (2)
#define LINFLEX_DIV_F_BAUD_RATE_460800              (3)
#define LINFLEX_DIV_M_BAUD_RATE_921600              (1)
#define LINFLEX_DIV_F_BAUD_RATE_921600              (1)
#define IRQ_BSP_RS232_RXI                           (79)
#define IRQ_BSP_RS232_TXI                           (80)
#define IRQ_BSP_RS232_ERR                           (81)
#define INTC_PRIORITY_BSP_RS232_RXI                 (2)
#define INTC_PRIORITY_BSP_RS232_TXI                 (2)
#define INTC_PRIORITY_BSP_RS232_ERR                 (2)
#define PCR_BSP_RS232_TX                            (18)
#define PCR_BSP_RS232_RX                            (19)
#define UART_TDFL_MAX                               (4)
#define UART_RDFL_MAX                               (4)
#define F_PERIPH_SET_1_CLK                          (16000000L)

/*
**************************************************************************************************************
*                                            OSTickISR
**************************************************************************************************************
*/

#define INTC_PRIORITY_BSP_OSTickISR                 (1)
#define IRQ_BSP_OSTickISR                           (60)
#define PIT_CH_BSP_OSTickISR                        (1)

/*
**************************************************************************************************************
*                                            DSPI
**************************************************************************************************************
*/

#define IRQ_DSPI_1_SR_TFUF_RFOF                     (94)
#define IRQ_DSPI_1_SR_EOQF                          (95)
#define IRQ_DSPI_1_SR_TFFF                          (96)
#define IRQ_DSPI_1_SR_TCF                           (97)
#define IRQ_DSPI_1_SR_RFDF                          (98)
#define INTC_PRIORITY_DSPI_1_SR_TFUF_RFOF           (6)
#define INTC_PRIORITY_DSPI_1_SR_EOQF                (6)
#define INTC_PRIORITY_DSPI_1_SR_TFFF                (6)
#define INTC_PRIORITY_DSPI_1_SR_TCF                 (6)
#define INTC_PRIORITY_DSPI_1_SR_RFDF                (6)
#define DSPI_PUSHR_MAX_BYTE_AMOUNT                  (2)
#define DSPI_TXFR_AMOUNT                            (4)
#define DSPI_ASYNC_SEND_DATA_MAX_LENGTH             ((DSPI_PUSHR_MAX_BYTE_AMOUNT) * (DSPI_TXFR_AMOUNT))
#define DSPI_CTAR_FMSZ_2BYTES                       (0b1111)
#define DSPI_CTAR_FMSZ_1BYTE                        (0b0111)
#define DSPI_1_MUTEX_PRIO                           (5)
extern struct DSPI_Device_Data
{
    uint32_t block[32];
    OS_EVENT *Mut_DSPI_1;
    int is_open;
    void(*CB_TX_Complete)(void);
    union {
        uint32_t R;
        struct {
            uint32_t DBR:1;
            uint32_t FMSZ:4;
            uint32_t CPOL:1;
            uint32_t CPHA:1;
            uint32_t LSBFE:1;
            uint32_t PCSSCK:2;
            uint32_t PASC:2;
            uint32_t PDT:2;
            uint32_t PBR:2;
            uint32_t CSSCK:4;
            uint32_t ASC:4;
            uint32_t DT:4;
            uint32_t BR:4;
        } B;
    } CTAR;
    union {
        uint32_t R;
        struct {
            uint32_t CONT:1;
            uint32_t CTAS:3;
            uint32_t EOQ:1;
            uint32_t CTCNT:1;
              uint32_t:4;
            uint32_t PCS:6;
            uint32_t TXDATA:16;
        } B;
    } PUSHR;
    volatile struct DSPI_tag *dspi;
} DSPI_1_Device_Data;

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
extern void  INTC_Handler_BSP_S3_S4(void);

/*
**************************************************************************************************************
*                                            UART SERVICES
**************************************************************************************************************
*/

extern struct UART_Buffer
{
    int length;
    int used;
    int DTF_err_cnt;
    uint8_t data[UART_RDFL_MAX];
};

extern struct   UART_Buffer UART_Buffer_0;
extern OS_EVENT *Sem_UART_0_TXI, *Sem_UART_0_RXI;
extern int      Init_UART_0_Ex(void);
extern int      Init_UART(volatile struct LINFLEX_tag *uart);
extern int      Set_UART_RDFL(volatile struct LINFLEX_tag *uart, int rdfl);
extern int      Set_UART_0_Pin(void);
extern int      Set_UART_Baud_Rate(volatile struct LINFLEX_tag *uart, int32_t baudrate);
extern int      Set_UART_Baud_Rate_Ex(volatile struct LINFLEX_tag *uart, int32_t baudrate);
extern int      Set_UART_0_INTC_Handler(INTCInterruptFn handler_rxi, INTCInterruptFn handler_txi, INTCInterruptFn handler_err);
extern int      Enable_UART_RXI(volatile struct LINFLEX_tag *uart);
extern int      Disable_UART_RXI(volatile struct LINFLEX_tag *uart);
extern int      Enable_UART_TXI(volatile struct LINFLEX_tag *uart);
extern int      Disable_UART_TXI(volatile struct LINFLEX_tag *uart);
extern int      Post_Date_to_UART_Buffer(volatile struct LINFLEX_tag *uart, const uint8_t data[], int cnt);
extern void     INTC_Handler_BSP_UART_0_RXI(void);
extern void     INTC_Handler_BSP_UART_0_TXI(void);
extern void     INTC_Handler_BSP_UART_0_ERR(void);

/*
**************************************************************************************************************
*                                            OSTickISR
**************************************************************************************************************
*/

extern void Init_OSTickISR(void);
extern void INTC_Handler_OSTickISR(void);

/*
**************************************************************************************************************
*                                            DSPI
**************************************************************************************************************
*/

extern int Init_SPI(volatile struct DSPI_tag *dspi);
extern int Init_DSPI_1(void);
extern int Set_DSPI_1_Pin(void);
extern void INTC_Handler_DSPI_1_SR_TFUF_RFOF(void);
extern void INTC_Handler_DSPI_1_SR_EOQF(void);
extern void INTC_Handler_DSPI_1_SR_TFFF(void);
extern void INTC_Handler_DSPI_1_SR_TCF(void);
extern void INTC_Handler_DSPI_1_SR_RFDF(void);
extern int Enable_INTC_DSPI_SR_TFUF(volatile struct DSPI_tag *dspi);
extern int Disable_INTC_DSPI_SR_TFUF(volatile struct DSPI_tag *dspi);
extern int Enable_INTC_DSPI_SR_RFOF(volatile struct DSPI_tag *dspi);
extern int Disable_INTC_DSPI_SR_RFOF(volatile struct DSPI_tag *dspi);
extern int Enable_INTC_DSPI_SR_EOQF(volatile struct DSPI_tag *dspi);
extern int Disable_INTC_DSPI_SR_EOQF(volatile struct DSPI_tag *dspi);
extern int Enable_INTC_DSPI_SR_TFFF(volatile struct DSPI_tag *dspi);
extern int Disable_INTC_DSPI_SR_TFFF(volatile struct DSPI_tag *dspi);
extern int Enable_INTC_DSPI_SR_SR_TCF(volatile struct DSPI_tag *dspi);
extern int Disable_INTC_DSPI_SR_SR_TCF(volatile struct DSPI_tag *dspi);
extern int Enable_INTC_DSPI_SR_SR_RFDF(volatile struct DSPI_tag *dspi);
extern int Disable_INTC_DSPI_SR_SR_RFDF(volatile struct DSPI_tag *dspi);
extern int Set_DSPI_CTAR(struct DSPI_Device_Data *dev, int dbr, int cpol, int cpha,int lsbfe,int pcssck,int pasc,int pdt,int pbr,int cssck,int asc,int dt,int br);
extern int Set_DSPI_PUSHR(struct DSPI_Device_Data *dev, int cont, int pcs);
extern int DSPI_ASYNC_Send_Data(struct DSPI_Device_Data *dev, uint8_t data[], int cnt);

/*
*********************************************************************************************************
*                                            MISCELLANEOUS
*********************************************************************************************************
*/

extern void  Delay_us(uint32_t us);
extern void  Delay_ms(uint32_t ms);
extern void  Disable_Watchdog(void);
extern void  Enable_IRQ(void);
extern void  Init_ModesAndClock(void);
extern void  Dummy(void *);


#ifndef CPU_CONST_FREQ
  #error   "BSP.H, Missing CPU_CONST_FREQ: Enable (1) or Disable (0) automatic CPU frequency check at CPU_CLK_FREQ()"
#endif

